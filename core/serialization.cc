// Copyright (c) 2014 The Caroline authors. All right reserved.
// Use of this source file is governed by a MIT license that can be found in the
// License file
/// @author: Vladislav Krikunov <vlad.krikunoff@yandex.ru>

#include "core/serialization.h"

#include <string>
#include <utility>
#include <vector>

#include "base/stream.h"
#include "google/protobuf/message_lite.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"

namespace bitdata {

namespace {

const char kOStreamConfigFieldName[] = "connection";
const char kIStreamConfigFieldName[] = "brain";
const char kCompressLevelConfigFieldName[] = "compression";
const int  kDefaultCompressLevel = 95;

} //namespace

bool GlobalMessage::SetOStream(const core::Config* settings) {
  const Json::Value* dictionary = settings->dictionary();
  if (!(dictionary && dictionary->isMember(kOStreamConfigFieldName))) {
    const Json::Value& address_node = (*dictionary)[kOStreamConfigFieldName];
    if(!address_node.isString()) {
      const std::string& ostream = address_node.asString();
      ostream_name_ = ostream;
      compress_level_ = kDefaultCompressLevel;
      if(dictionary->isMember(kCompressLevelConfigFieldName)) {
        const Json::Value compress_node = (*dictionary)[kCompressLevelConfigFieldName];
        if(compress_node.isInt())
          compress_level_ = compress_node.asInt();
      }
      ostream_ = base::Stream::Open(ostream_name_, base::Stream::kWrite);
      return true;
    }
  }
  return false;
}

bool GlobalMessage::SetIStream(const core::Config* settings) {
  const Json::Value* dictionary = settings->dictionary();
  if(dictionary && dictionary->isMember(kIStreamConfigFieldName)) {
    const Json::Value& address_node = (*dictionary)[kIStreamConfigFieldName];
    if (address_node.isString()) {
      const std::string istream = address_node.asString();
      istream_name_ = istream;
      istream_ = base::Stream::Open(istream_name_, base::Stream::kRead);
      return true;
    }
  }
  return false;
}

void GlobalMessage::MakeMessage(Message* message) {
  size_t size = message->ByteSize();
  int size_of_size = google::protobuf::io
                     ::CodedOutputStream::VarintSize32(size);
  std::unique_ptr<char[]>bit_message(new char[size + size_of_size]);
  google::protobuf::io::ArrayOutputStream stream_message(bit_message.get(),
                                                         size + size_of_size);
  google::protobuf::io::CodedOutputStream coded_message(&stream_message);
  coded_message.WriteVarint32(size);
  message->SerializeToCodedStream(&coded_message);
  ostream_->Write(bit_message.get(), size + size_of_size);
}

void GlobalMessage::GenDMap(const core::DepthMap& depth_map) {
  std::unique_ptr<Message> message(new Message());
  message->mutable_depth_map()->set_width(depth_map.width());
  message->mutable_depth_map()->set_height(depth_map.height());
  for (double depth : depth_map)
    message->mutable_depth_map()->add_data(depth);
  message->set_type(Message::DEPTH_MAP);
  this->MakeMessage(message.get());
}

void GlobalMessage::GenOptFlow(const core::OpticalFlow& optical_flow) {
  std::unique_ptr<Message>message(new Message);
  for (auto pair : optical_flow) {
    OpticalFlowItem* item = message->mutable_optical_flow()->add_item();
    item->mutable_left()->set_x(pair.first.x);
    item->mutable_left()->set_y(pair.first.y);
    item->mutable_right()->set_x(pair.second.x);
    item->mutable_right()->set_y(pair.second.y);
  }
  message->set_type(Message::OPTICAL_FLOW);
  this->MakeMessage(message.get());
}

void GlobalMessage::GenModel(const core::Scene3D& scene) {
  std::unique_ptr<Message>message(new Message());
  std::vector<cv::Point3d> vertexes;
  std::vector<core::Triangle> faces;
  core::SceneElement some_element;
  int vertex_base_index = 0;
  for (auto scene_element : scene) {
    for (auto vertex : scene_element->mesh()->vertexes()) {
      auto transformed_vertex = vertex;
      scene_element->Transform(&transformed_vertex, transformed_vertex);
      auto proto_vertex = message->mutable_model()->add_verticies();
      proto_vertex->set_x(vertex.x);
      proto_vertex->set_y(vertex.y);
      proto_vertex->set_z(vertex.z);
    }
    for (auto face : scene_element->mesh()->faces()) {
      auto proto_face = message->mutable_model()->add_faces();
      proto_face->set_point1(face.Point1() + vertex_base_index);
      proto_face->set_point2(face.Point2() + vertex_base_index);
      proto_face->set_point3(face.Point3() + vertex_base_index);
    }
    vertex_base_index += scene_element->mesh()->vertexes().size();
  }
  message->set_type(Message::MODEL);
  this->MakeMessage(message.get());
}

void GlobalMessage::GenPic(const std::vector<std::pair<cv::Mat,
            core::Position>>& frameset) {
  std::unique_ptr<Message>message(new Message());
  std::vector<uint8_t> left, right;
  std::vector<int> prop;
  prop.push_back(cv::IMWRITE_JPEG_QUALITY);
  prop.push_back(95);
  cv::imencode("*.jpeg", frameset[0].first, left, prop);
  cv::imencode("*.jpeg", frameset[1].first, right, prop);
  message->mutable_images()->mutable_left()->
      set_width(frameset[0].first.size().width);
  message->mutable_images()->mutable_left()->
      set_height(frameset[0].first.size().height);
  message->mutable_images()->mutable_right()->
      set_width(frameset[1].first.size().width);
  message->mutable_images()->mutable_right()->
      set_height(frameset[1].first.size().height);

  auto it = left.begin(), end = left.end();
  while (it != end) {
    message->mutable_images()->mutable_left()->add_data(*(it++));
  }
  it = right.begin(); end = right.end();
  while (it != end) {
    message->mutable_images()->mutable_right()->add_data(*(it++));
  }
  message->set_type(Message::IMAGES);
  this->MakeMessage(message.get());
}

void GlobalMessage::GenLog(const std::string& message) {
  std::unique_ptr<Message>proto_message(new Message());
  proto_message->mutable_log()->set_line(message);
  proto_message->set_type(Message::LOG);
  this->MakeMessage(proto_message.get());
}

void GlobalMessage::Observe(const std::string& message) {
  GenLog(message);
}

}  // namespace bitdata
