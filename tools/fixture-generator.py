#!/usr/bin/env python3

import json
from optparse import OptionParser
import os
import shutil
import random
import math

import bpy

class Bounds(object):
  def __init__(self, camera):
    self.angle_x = camera.angle_x
    self.angle_y = camera.angle_y
    self.angle = camera.angle
    self.clip_near = camera.clip_start
    self.clip_far = camera.clip_end

def random_mesh_origin(angle_x, angle_y, clip_near, clip_far):
  """Create random origin point for mesh. Assume that object will be visible if
     it's origin is in the truncated pyramid."""
  # Get z coordinate first because x and y ranges depends from it.
  z = random.uniform(clip_near + 2, clip_far - 2)
  x = random.uniform(-math.tan(angle_x / 2) * z,
      math.tan(angle_x / 2) * z / 2)
  y = random.uniform(-math.tan(angle_y / 2) * z,
      math.tan(angle_y / 2) * z / 2)
  return [x, y, -z]

def random_mesh_rotation():
  """Generate random angles for initial mesh rotation."""
  return [random.random(), random.random(), random.random()];

def random_mesh_constructor():
  """Returs lambda for creating a random mesh with given origin point and
     rotation."""
  def dimension(max_size):
    """Return random value for dimension."""
    return random.uniform(max_size / 10, max_size)

  return random.choice((
      lambda origin, rotation, max_size:
       bpy.ops.mesh.primitive_cone_add(radius1 = dimension(max_size) / 2,
           radius2 = dimension(max_size) / 2, depth = dimension(max_size),
           end_fill_type='TRIFAN',
           location=origin, rotation=rotation),
      lambda origin, rotation, max_size:
       bpy.ops.mesh.primitive_cube_add(radius = dimension(max_size) / 2,
           location = origin, rotation = rotation),
      lambda origin, rotation, max_size:
       bpy.ops.mesh.primitive_cylinder_add(radius = dimension(max_size) / 2,
           depth = dimension(max_size),
           location = origin, rotation = rotation),
      lambda origin, rotation, max_size:
       bpy.ops.mesh.primitive_ico_sphere_add(size = dimension(max_size) / 2,
           location = origin, rotation = rotation),
      lambda origin, rotation, max_size:
       bpy.ops.mesh.primitive_monkey_add(radius = dimension(max_size) / 2,
           location = origin, rotation = rotation),
      lambda origin, rotation, max_size:
       bpy.ops.mesh.primitive_plane_add(radius = dimension(max_size) / 2,
           location = origin, rotation = rotation),
      lambda origin, rotation, max_size:
       bpy.ops.mesh.primitive_torus_add(
           major_radius = random.uniform(max_size / 4, max_size) / 2,
           minor_radius = dimension(max_size / 5) / 2,
           location = origin, rotation = rotation),
      lambda origin, rotation, max_size:
       bpy.ops.mesh.primitive_uv_sphere_add(size = dimension(max_size) / 2,
           location = origin, rotation = rotation)
    ))

def override_context(options):
  """Copy bpy.context and replace some values by provided in dictionary."""
  context = bpy.context.copy()
  for key in options.keys():
    context[key] = options[key]
  return context

def parse_options():
  """Configure and run command line arguments parser."""
  parser = OptionParser()
  parser.add_option('-f', '--fixtures', dest = 'count', type = 'int',
      default = 1, action = 'store',
      help = 'Number of test fixtures to generate.')
  parser.add_option('-m', '--meshes', dest = 'meshes', type = 'int',
      default = 1, action = 'store',
      help = 'Maximum number of meshes in the scene.')
  parser.add_option('-o', '--output', dest = 'output', type = 'string',
      default = 'fixture', action = 'store',
      help = 'Name of the output directory. You can use %n for fixture number.'),
  parser.add_option('-d', '--distance', dest = 'distance', type = 'float',
      default = 0.08, action = 'store',
      help = 'Distantion between cameras in meters.')
  (options, args) = parser.parse_args()
  if options.count < 1:
    parser.error('Count of fixtures must be positive integer number.')
  if options.meshes < 1:
    parser.error('Maximum count of meshes must be positive integer number.')
  if options.count > 1 and '%n' not in options.output:
    options.output += '.%n'
  if options.distance <= 0:
    parser.error('Distance must be greater that 0.')
  options.output = options.output.replace('%n', '{number}')
  return (options, args);

def get_camera():  
  camera = None
  for cam in bpy.data.cameras:
    if cam.name in bpy.context.scene.objects.keys():
      camera = cam
      break
  camera_object = bpy.data.objects[camera.name]
  return (camera, camera_object)


def cleanup_scene():
  """Cleanup current scene"""
  for item in bpy.data.meshes:
    if item.name in bpy.context.scene.objects.keys():
      bpy.context.scene.objects.unlink(bpy.data.objects[item.name])

def generate_mesh(bounds):
  """Count location and rotation for mesh, get and call constructor."""
  origin = random_mesh_origin(bounds.angle_x, bounds.angle_y,
      bounds.clip_near, bounds.clip_far)
  rotation = random_mesh_rotation()
  max_size = - origin[2] * math.tan(bounds.angle / 2)
  random_mesh_constructor()(origin=origin, rotation=rotation,
      max_size=max_size)

def generate_meshes(count, bounds):
  """Generate meshes on the scene."""
  for i in range(count):
    generate_mesh(bounds = bounds)

def setup_light(bounds):
  """Find and set lamp to the appropriate location."""
  lamp = bpy.data.lamps[0]
  if not lamp:
    lamp = bpy.data.lamps.new(name='lamp', type='SUN')
  lamp_object = bpy.data.objects[lamp.name]
  if lamp.name not in bpy.context.scene.objects:
    bpy.context.scene.objects.link(lamp_object)

  lamp_location = [
      0.0,
      math.tan(bounds.angle) * bounds.clip_far,
      -(bounds.clip_far - bounds.clip_near) / 2
    ]
  lamp.type = 'SUN'
  lamp_object.location = lamp_location
  lamp_object.rotation_mode = 'XYZ'
  lamp_object.rotation_euler = [0, - math.pi / 2, 0]

def prepare_dir(name):
  """Remove fixture directory if it exists. Create new one."""
  if os.path.exists(name):
    shutil.rmtree(name)
    
  os.mkdir(name)

def save_models(name):
  """Save each mesh to the Standford ply file. Return scene dictionary for
     config."""
  scene = {}
  for mesh in bpy.data.meshes:
    if mesh.name in bpy.context.scene.objects.keys():
      mesh_ply = mesh.name + '.ply'
      bpy.ops.export_mesh.ply(override_context({'selected_objects': mesh}),
          filepath=os.path.join(name, mesh_ply), check_existing=False,
          use_mesh_modifiers=False, use_normals=False, use_uv_coords=False,
          use_colors=False)
      scene[mesh.name] = mesh_ply
  return scene

def render_frames(name, distance, camera, camera_object, bounds):
  """Setup camera and render two frames."""
  cameras = []
  for i, location in enumerate([
    [-distance / 2, 0, 0],
    [distance / 2, 0, 0]
  ]):
    render = bpy.context.scene.render
    width = render.resolution_x
    height = render.resolution_y
    cam_name = 'camera_' + str(i)
    render.filepath = os.path.join(name, cam_name + '.png')
    camera_object.location = location
    camera_object.rotation_mode = 'XYZ'
    camera_object.rotation_euler = [0, 0, 0]
    bpy.ops.render.render(animation=False, write_still=True)
    cam = {
      'focus': camera.lens / 1000.0,
      'pixel_size': camera.sensor_width / width / 1000.0,
      'width': width,
      'height': height,
      'position': camera_object.location[:],
      'rotation': camera_object.rotation_euler[:],
      'type': 'image',
      'source': camera.name + '.png'
    }
    cameras.append(cam)
  return cameras

def save_config(name, config):
  """Generate config.json for given fixture."""
  config_file = open(os.path.join(name, 'config.json'), 'w')
  json.dump(config, config_file, skipkeys=True, ensure_ascii=False,
      allow_nan=False, indent=2, sort_keys=True)
  config_file.write('\n')

def save_fixture(name, distance, camera, camera_object, bounds):
  prepare_dir(name = name)

  config = {}
  config['scene'] = save_models(name = name)

  config['cameras'] = render_frames(name = name, distance = distance,
      camera = camera, camera_object = camera_object, bounds = bounds)

  save_config(name = name, config = config)

def generate_fixture(name, meshes, distance):
  """Generate and save a fixture."""
  
  name = os.path.normpath(name)
  
  (camera, camera_object) = get_camera()
  bounds = Bounds(camera)

  cleanup_scene()

  generate_meshes(count = meshes, bounds = bounds)
  setup_light(bounds = bounds)

  save_fixture(name = name, distance = distance, camera=camera,
      camera_object=camera_object, bounds=bounds)

def generate_fixtures(count, output, meshes, distance):
  """Generate all fixtures."""
  for i in range(count):
    generate_fixture(name = output.format(number = i),
        meshes = random.randrange(meshes) + 1, distance = distance)

def main():
  """Entry point."""
  (options, args) = parse_options()
  generate_fixtures(count = options.count, output = options.output,
      meshes = options.meshes, distance = options.distance)

if __name__ == '__main__':
  main()
