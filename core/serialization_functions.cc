// Copyright (c) 2014 The Caroline authors. All right reserved.
// Use of this source file is governed by a MIT license that can be found in the// License file
// Author: Vladislav Krikunov <vlad.krikunoff@yandex.ru>
#include "core/depth_map.h"
#include "core/mesh.h"
#include "core/scene3d.h"
#include "core/point3d.h"
#include "core/image_capture_impl.h"

#include "protocol.pb.h"

namespace bitdata 
{
inline Message* GenDMap (core::DepthMap& defaultM)
{
  DepthMap* new_map = new DepthMap;
  Message* mess = new Message;
  int height,width;
  height = defaultM.height();
  width = defaultM.width();
  new_map->set_width(height);
  new_map->set_height(width);
  int j,i,k(0);
  for(int i=0;i<height;i++)
    {
      for( j=0;j<width;j++)
        new_map->set_data(i+j,defaultM.Depth(i+k,j));
      k+=width;
    }
  mess->set_allocated_depth_map(new_map);
  mess->set_type(bitdata::Message::DEPTH_MAP);
  delete new_map;
  return mess;
}

Message* GenOptFlow (core::OpticalFlow defaultFlow)
{
  int i=0;
  Point2 point1,point2;
  OpticalFlowItem flowItem;
  OpticalFlow newFlow;
  Message* mess = new Message;
  do {
    newFlow.add_item();
    newFlow.mutable_item(i)->
        mutable_left()->set_x(defaultFlow[i].first.x);
    newFlow.mutable_item(i)->
        mutable_left()->set_y(defaultFlow[i].first.y);
    newFlow.mutable_item(i)->
        mutable_right()->set_x(defaultFlow[i].second.x);
    newFlow.mutable_item(i)->
        mutable_right()->set_y(defaultFlow[i].second.y);
    i++;
  }
  while(i<defaultFlow.size());
  mess->set_allocated_optical_flow(&newFlow);
  mess->set_type(bitdata::Message::OPTICAL_FLOW);
  return mess; 
}

Message* GenModel(core::Scene3D defaultScene)
{
  Message* mess = new Message;
  int i = defaultScene.NumberOfElements();
  unsigned int j=0,k,n;
  std::vector<core::Point3D>vertexes;
  std::vector<core::Triangle>faces;
  Model new_model;
  Point3 new_point;
  Face new_face;
  core::SceneElement some_element;
  do
  {
    some_element = some_element.Transform(defaultScene.ElementAt(j)); 
    vertexes = some_element.get_mesh()->Vertexes();
    faces = some_element.get_mesh()->Faces();
    k = vertexes.size();
    for(j=0;j<k;j++)
    {
      new_model.add_verticies();
      new_model.mutable_verticies(j)->set_x(vertexes[n].x());
      new_model.mutable_verticies(j)->set_y(vertexes[n].y());
      new_model.mutable_verticies(j)->set_z(vertexes[n].z());
    }
    k = faces.size();
    for(n=0;n<k;n++)
    {
      new_model.add_faces();
      new_model.mutable_faces(n)->set_point1(faces[n].Point1());
      new_model.mutable_faces(n)->set_point2(faces[n].Point2());
      new_model.mutable_faces(n)->set_point3(faces[n].Point3());
    }
    j++;
  }
  while(j<i);
  mess->set_allocated_model(&new_model);
  mess->set_type(bitdata::Message::MODEL);
  return mess;
  delete mess;
}

Message* GenPic(core::ImageCapture* default_pic1,
                          core::ImageCapture* default_pic2 )
{
  Images* new_img = new Images;
  Image new_pic1,new_pic2;
  Message* mess;
  cv::Mat frame1,frame2;
  int height,width;
  int n,k,z(0);
  frame1 = default_pic1->GetNextImage();
  frame2 = default_pic1->GetNextImage();
  height= frame1.size().height;
  width = frame1.size().width;
  new_pic1.set_width(width);
  new_pic2.set_height(height);
  for(n=0;n<height;n++)
    { for(k=0;k<width;k++)
      {
        new_pic1.set_data(k+z,frame1.at<int>(k,n));
        new_pic2.set_data(k+z,frame2.at<int>(k,n));
      }
      z+=width;
    }
  new_img->set_allocated_left(&new_pic1);
  new_img->set_allocated_right(&new_pic1);        
  mess->set_allocated_images(new_img);
  mess->set_type(bitdata::Message::IMAGES);
  return mess;
  delete mess;
}

} //namespace bitdata
  




    
  
  




 
