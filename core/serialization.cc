// Copyright (c) 2014 The Caroline authors. All right reserved.
// Use of this source file is governed by a MIT license that can be found in the// License file
/// @author: Vladislav Krikunov <vlad.krikunoff@yandex.ru>

#include "core/serialization.h"

namespace bitdata {

inline void GenDMap (core::DepthMap& defaultM) {
  Message* mess = new Message;
  int height,width;
  height = defaultM.height();
  width = defaultM.width();
  mess->mutable_depth_map()->set_width(width);
  mess->mutable_depth_map()->set_height(width);
  int j;
  int i;
  int k = 0;
  for(int i=0;i<height;i++) { 
    for( j=0;j<width;j++) {
      mess->mutable_depth_map()->set_data(i+j,defaultM.Depth(i+k,j));
    }
    k += width;
  }
  mess->set_type(bitdata::Message::DEPTH_MAP);
}

void GenOptFlow (core::OpticalFlow defaultFlow){
  int i=0;
  OpticalFlowItem flowItem;
  std::unique_ptr<Message>mess(new Message);
  do {
    mess->mutable_optical_flow()->mutable_item(i)->
        mutable_left()->set_x(defaultFlow[i].first.x);
    mess->mutable_optical_flow()->mutable_item(i)->
        mutable_left()->set_y(defaultFlow[i].first.y);
    mess->mutable_optical_flow()->mutable_item(i)->
        mutable_right()->set_x(defaultFlow[i].second.x);
    mess->mutable_optical_flow()->mutable_item(i)->
        mutable_right()->set_y(defaultFlow[i].second.y);
    i++;
  }
  while(i<defaultFlow.size());
  mess->set_type(bitdata::Message::OPTICAL_FLOW);
}

void GenModel(core::Scene3D defaultScene){
  std::unique_ptr<Message>mess(new Message);
  int i = defaultScene.NumberOfElements();
  unsigned int j=0,k,n;
  std::vector<core::Point3D>vertexes;
  std::vector<core::Triangle>faces;
  core::SceneElement some_element;
  do {  
    some_element = some_element.Transform(defaultScene.ElementAt(j)); 
    vertexes = some_element.get_mesh()->Vertexes();
    faces = some_element.get_mesh()->Faces();
    k = vertexes.size();
    for(n=0;j<k;n++){
      mess->mutable_model()->mutable_verticies(n)->
          set_x(vertexes[n].x());
      mess->mutable_model()->mutable_verticies(n)->
          set_y(vertexes[n].y());
      mess->mutable_model()->mutable_verticies(n)->
          set_z(vertexes[n].z());                
    }
    k = faces.size();
    for(n=0;n<k;n++) {  
    mess->mutable_model()->mutable_faces(n)->
        set_point1(faces[n].Point1());
    mess->mutable_model()->mutable_faces(n)->
        set_point2(faces[n].Point2());
    mess->mutable_model()->mutable_faces(n)->
        set_point3(faces[n].Point3());
    }
    j++;
  }
  while(j<i);
  mess->set_type(bitdata::Message::MODEL);
}

void GenPic(core::ImageCapture* default_pic1,
            core::ImageCapture* default_pic2 ) {
  std::unique_ptr<Message>mess(new Message);
  cv::Mat frame1;
  cv::Mat frame2;
  int height;
  int width;
  int n;
  int k;
  int z=0;
  frame1 = default_pic1->GetNextImage();
  frame2 = default_pic1->GetNextImage();
  height= frame1.size().height;
  width = frame1.size().width;
  mess->mutable_images()->mutable_left()->
      set_width(width);
  mess->mutable_images()->mutable_right()->
      set_height(height);
  for(n=0;n<height;n++) {
     for(k=0;k<width;k++){
      mess->mutable_images()->mutable_left()->
          set_data(k+z,frame1.at<int>(k,n));
      mess->mutable_images()->mutable_right()->
          set_data(k+z,frame2.at<int>(k,n));
      }
      z+=width;
  }
  mess->set_type(bitdata::Message::IMAGES);
}

} //namespace bitdata
  
