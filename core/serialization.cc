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
#include "protocol.pb.h"  // NOLINT

namespace bitdata {


void GlobalMessage::SetStream(const std::string& stream) {
  stream_name_ = stream;
  stream_ = base::Stream::Open(stream_name_, base::Stream::kWrite);
}

void GlobalMessage::GenDMap(const core::DepthMap& depth_map) {
  std::unique_ptr<Message> message(new Message());
  message->mutable_depth_map()->set_width(depth_map.width());
  message->mutable_depth_map()->set_height(depth_map.height());
  for (double depth : depth_map)
    message->mutable_depth_map()->add_data(depth);
  message->set_type(Message::DEPTH_MAP);
  int intsize = google::protobuf::io::CodedOutputStream::VarintSize32(message->ByteSize());
  size_t bit_size = message->ByteSize() + intsize;
  char* bit_message = new char[bit_size];
  google::protobuf::io::ArrayOutputStream stream_message(bit_message,bit_size);
  google::protobuf::io::CodedOutputStream coded_message(&stream_message);
  coded_message.WriteVarint32(message->GetCachedSize());
  message->SerializeToCodedStream(&coded_message);
  stream_->Write(bit_message, bit_size);
  delete bit_message;
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
  int intsize = google::protobuf::io::CodedOutputStream::VarintSize32(message->ByteSize());
  size_t bit_size = message->ByteSize() + intsize;
  char* bit_message = new char[bit_size];
  google::protobuf::io::ArrayOutputStream stream_message(bit_message,bit_size);
  google::protobuf::io::CodedOutputStream coded_message(&stream_message);
  coded_message.WriteVarint32(message->GetCachedSize());
  message->SerializeToCodedStream(&coded_message);
  stream_->Write(bit_message, bit_size);
  delete bit_message;
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
  int intsize = google::protobuf::io::CodedOutputStream::VarintSize32(message->ByteSize());
  size_t bit_size = message->ByteSize() + intsize;
  char* bit_message = new char[bit_size];
  google::protobuf::io::ArrayOutputStream stream_message(bit_message,bit_size);
  google::protobuf::io::CodedOutputStream coded_message(&stream_message);
  coded_message.WriteVarint32(message->GetCachedSize());
  message->SerializeToCodedStream(&coded_message);
  stream_->Write(bit_message, bit_size);
  delete bit_message;
}

void GlobalMessage::GenPic(const std::vector<std::pair<cv::Mat,
            core::Position>>& frameset) {
  std::unique_ptr<Message>message(new Message());
  const cv::Mat& left = frameset[0].first;
  const cv::Mat& right = frameset[1].first;
  message->mutable_images()->mutable_left()->
      set_width(left.size().width);
  message->mutable_images()->mutable_left()->
      set_height(left.size().height);
  message->mutable_images()->mutable_right()->
      set_width(right.size().width);
  message->mutable_images()->mutable_right()->
      set_height(right.size().height);
  for (auto it = left.begin<int>(), end = left.end<int>();
      it != end; ++it) {
    message->mutable_images()->mutable_left()->add_data(*it);
  }
  for (auto it = right.begin<int>(), end = right.end<int>();
      it != end; ++it) {
    message->mutable_images()->mutable_right()->add_data(*it);
  }
  message->set_type(Message::IMAGES);
  int intsize = google::protobuf::io::CodedOutputStream::VarintSize32(message->ByteSize());
  size_t bit_size = message->ByteSize() + intsize;
  char* bit_message = new char[bit_size];
  google::protobuf::io::ArrayOutputStream stream_message(bit_message,bit_size);
  google::protobuf::io::CodedOutputStream coded_message(&stream_message);
  coded_message.WriteVarint32(message->GetCachedSize());
  message->SerializeToCodedStream(&coded_message);
  stream_->Write(bit_message, bit_size);
  delete bit_message;
}

void GlobalMessage::GenLog(const std::string& message) {
  std::unique_ptr<Message>proto_message(new Message());
  proto_message->mutable_log()->set_line(message);
  proto_message->set_type(Message::LOG);
  int intsize = google::protobuf::io::CodedOutputStream::VarintSize32(proto_message->ByteSize());
  size_t bit_size = proto_message->ByteSize() + intsize;
  char* bit_message = new char[bit_size];
  google::protobuf::io::ArrayOutputStream stream_message(bit_message,bit_size);
  google::protobuf::io::CodedOutputStream coded_message(&stream_message);
  coded_message.WriteVarint32(proto_message->GetCachedSize());
  proto_message->SerializeToCodedStream(&coded_message);
  stream_->Write(bit_message, bit_size);
  delete bit_message;
}

void GlobalMessage::Observe(const std::string& message) {
  GenLog(message);
}

}  // namespace bitdata

