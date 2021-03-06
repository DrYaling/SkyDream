// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: punch.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "punch.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace SkyDream {

namespace {

const ::google::protobuf::Descriptor* Person_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Person_reflection_ = NULL;
const ::google::protobuf::Descriptor* ListConn_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ListConn_reflection_ = NULL;
const ::google::protobuf::Descriptor* C2S_Punch_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  C2S_Punch_reflection_ = NULL;
const ::google::protobuf::Descriptor* IntValue_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  IntValue_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_punch_2eproto() {
  protobuf_AddDesc_punch_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "punch.proto");
  GOOGLE_CHECK(file != NULL);
  Person_descriptor_ = file->message_type(0);
  static const int Person_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Person, ip_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Person, port_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Person, clientid_),
  };
  Person_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Person_descriptor_,
      Person::default_instance_,
      Person_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Person, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Person, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Person));
  ListConn_descriptor_ = file->message_type(1);
  static const int ListConn_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ListConn, persons_),
  };
  ListConn_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ListConn_descriptor_,
      ListConn::default_instance_,
      ListConn_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ListConn, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ListConn, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ListConn));
  C2S_Punch_descriptor_ = file->message_type(2);
  static const int C2S_Punch_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(C2S_Punch, from_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(C2S_Punch, to_),
  };
  C2S_Punch_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      C2S_Punch_descriptor_,
      C2S_Punch::default_instance_,
      C2S_Punch_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(C2S_Punch, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(C2S_Punch, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(C2S_Punch));
  IntValue_descriptor_ = file->message_type(3);
  static const int IntValue_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(IntValue, value_),
  };
  IntValue_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      IntValue_descriptor_,
      IntValue::default_instance_,
      IntValue_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(IntValue, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(IntValue, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(IntValue));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_punch_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Person_descriptor_, &Person::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ListConn_descriptor_, &ListConn::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    C2S_Punch_descriptor_, &C2S_Punch::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    IntValue_descriptor_, &IntValue::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_punch_2eproto() {
  delete Person::default_instance_;
  delete Person_reflection_;
  delete ListConn::default_instance_;
  delete ListConn_reflection_;
  delete C2S_Punch::default_instance_;
  delete C2S_Punch_reflection_;
  delete IntValue::default_instance_;
  delete IntValue_reflection_;
}

void protobuf_AddDesc_punch_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\013punch.proto\022\010SkyDream\"4\n\006Person\022\n\n\002ip\030"
    "\001 \002(\t\022\014\n\004port\030\002 \002(\005\022\020\n\010clientId\030\003 \002(\005\"-\n"
    "\010ListConn\022!\n\007persons\030\001 \003(\0132\020.SkyDream.Pe"
    "rson\"%\n\tC2S_Punch\022\014\n\004from\030\001 \002(\005\022\n\n\002to\030\002 "
    "\002(\005\"\031\n\010IntValue\022\r\n\005value\030\001 \002(\005", 190);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "punch.proto", &protobuf_RegisterTypes);
  Person::default_instance_ = new Person();
  ListConn::default_instance_ = new ListConn();
  C2S_Punch::default_instance_ = new C2S_Punch();
  IntValue::default_instance_ = new IntValue();
  Person::default_instance_->InitAsDefaultInstance();
  ListConn::default_instance_->InitAsDefaultInstance();
  C2S_Punch::default_instance_->InitAsDefaultInstance();
  IntValue::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_punch_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_punch_2eproto {
  StaticDescriptorInitializer_punch_2eproto() {
    protobuf_AddDesc_punch_2eproto();
  }
} static_descriptor_initializer_punch_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int Person::kIpFieldNumber;
const int Person::kPortFieldNumber;
const int Person::kClientIdFieldNumber;
#endif  // !_MSC_VER

Person::Person()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:SkyDream.Person)
}

void Person::InitAsDefaultInstance() {
}

Person::Person(const Person& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:SkyDream.Person)
}

void Person::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  port_ = 0;
  clientid_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Person::~Person() {
  // @@protoc_insertion_point(destructor:SkyDream.Person)
  SharedDtor();
}

void Person::SharedDtor() {
  if (ip_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete ip_;
  }
  if (this != default_instance_) {
  }
}

void Person::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Person::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Person_descriptor_;
}

const Person& Person::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_punch_2eproto();
  return *default_instance_;
}

Person* Person::default_instance_ = NULL;

Person* Person::New() const {
  return new Person;
}

void Person::Clear() {
#define OFFSET_OF_FIELD_(f) (reinterpret_cast<char*>(      \
  &reinterpret_cast<Person*>(16)->f) - \
   reinterpret_cast<char*>(16))

#define ZR_(first, last) do {                              \
    size_t f = OFFSET_OF_FIELD_(first);                    \
    size_t n = OFFSET_OF_FIELD_(last) - f + sizeof(last);  \
    ::memset(&first, 0, n);                                \
  } while (0)

  if (_has_bits_[0 / 32] & 7) {
    ZR_(port_, clientid_);
    if (has_ip()) {
      if (ip_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        ip_->clear();
      }
    }
  }

#undef OFFSET_OF_FIELD_
#undef ZR_

  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Person::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:SkyDream.Person)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string ip = 1;
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_ip()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->ip().data(), this->ip().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "ip");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(16)) goto parse_port;
        break;
      }

      // required int32 port = 2;
      case 2: {
        if (tag == 16) {
         parse_port:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &port_)));
          set_has_port();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(24)) goto parse_clientId;
        break;
      }

      // required int32 clientId = 3;
      case 3: {
        if (tag == 24) {
         parse_clientId:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &clientid_)));
          set_has_clientid();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:SkyDream.Person)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:SkyDream.Person)
  return false;
#undef DO_
}

void Person::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:SkyDream.Person)
  // required string ip = 1;
  if (has_ip()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->ip().data(), this->ip().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "ip");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->ip(), output);
  }

  // required int32 port = 2;
  if (has_port()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->port(), output);
  }

  // required int32 clientId = 3;
  if (has_clientid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->clientid(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:SkyDream.Person)
}

::google::protobuf::uint8* Person::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:SkyDream.Person)
  // required string ip = 1;
  if (has_ip()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->ip().data(), this->ip().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "ip");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->ip(), target);
  }

  // required int32 port = 2;
  if (has_port()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->port(), target);
  }

  // required int32 clientId = 3;
  if (has_clientid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->clientid(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:SkyDream.Person)
  return target;
}

int Person::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string ip = 1;
    if (has_ip()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->ip());
    }

    // required int32 port = 2;
    if (has_port()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->port());
    }

    // required int32 clientId = 3;
    if (has_clientid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->clientid());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Person::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Person* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Person*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Person::MergeFrom(const Person& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_ip()) {
      set_ip(from.ip());
    }
    if (from.has_port()) {
      set_port(from.port());
    }
    if (from.has_clientid()) {
      set_clientid(from.clientid());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Person::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Person::CopyFrom(const Person& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Person::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000007) != 0x00000007) return false;

  return true;
}

void Person::Swap(Person* other) {
  if (other != this) {
    std::swap(ip_, other->ip_);
    std::swap(port_, other->port_);
    std::swap(clientid_, other->clientid_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Person::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Person_descriptor_;
  metadata.reflection = Person_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int ListConn::kPersonsFieldNumber;
#endif  // !_MSC_VER

ListConn::ListConn()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:SkyDream.ListConn)
}

void ListConn::InitAsDefaultInstance() {
}

ListConn::ListConn(const ListConn& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:SkyDream.ListConn)
}

void ListConn::SharedCtor() {
  _cached_size_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ListConn::~ListConn() {
  // @@protoc_insertion_point(destructor:SkyDream.ListConn)
  SharedDtor();
}

void ListConn::SharedDtor() {
  if (this != default_instance_) {
  }
}

void ListConn::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ListConn::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ListConn_descriptor_;
}

const ListConn& ListConn::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_punch_2eproto();
  return *default_instance_;
}

ListConn* ListConn::default_instance_ = NULL;

ListConn* ListConn::New() const {
  return new ListConn;
}

void ListConn::Clear() {
  persons_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ListConn::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:SkyDream.ListConn)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .SkyDream.Person persons = 1;
      case 1: {
        if (tag == 10) {
         parse_persons:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_persons()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(10)) goto parse_persons;
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:SkyDream.ListConn)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:SkyDream.ListConn)
  return false;
#undef DO_
}

void ListConn::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:SkyDream.ListConn)
  // repeated .SkyDream.Person persons = 1;
  for (int i = 0; i < this->persons_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->persons(i), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:SkyDream.ListConn)
}

::google::protobuf::uint8* ListConn::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:SkyDream.ListConn)
  // repeated .SkyDream.Person persons = 1;
  for (int i = 0; i < this->persons_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->persons(i), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:SkyDream.ListConn)
  return target;
}

int ListConn::ByteSize() const {
  int total_size = 0;

  // repeated .SkyDream.Person persons = 1;
  total_size += 1 * this->persons_size();
  for (int i = 0; i < this->persons_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->persons(i));
  }

  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void ListConn::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ListConn* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ListConn*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ListConn::MergeFrom(const ListConn& from) {
  GOOGLE_CHECK_NE(&from, this);
  persons_.MergeFrom(from.persons_);
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ListConn::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ListConn::CopyFrom(const ListConn& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ListConn::IsInitialized() const {

  if (!::google::protobuf::internal::AllAreInitialized(this->persons())) return false;
  return true;
}

void ListConn::Swap(ListConn* other) {
  if (other != this) {
    persons_.Swap(&other->persons_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ListConn::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ListConn_descriptor_;
  metadata.reflection = ListConn_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int C2S_Punch::kFromFieldNumber;
const int C2S_Punch::kToFieldNumber;
#endif  // !_MSC_VER

C2S_Punch::C2S_Punch()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:SkyDream.C2S_Punch)
}

void C2S_Punch::InitAsDefaultInstance() {
}

C2S_Punch::C2S_Punch(const C2S_Punch& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:SkyDream.C2S_Punch)
}

void C2S_Punch::SharedCtor() {
  _cached_size_ = 0;
  from_ = 0;
  to_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

C2S_Punch::~C2S_Punch() {
  // @@protoc_insertion_point(destructor:SkyDream.C2S_Punch)
  SharedDtor();
}

void C2S_Punch::SharedDtor() {
  if (this != default_instance_) {
  }
}

void C2S_Punch::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* C2S_Punch::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return C2S_Punch_descriptor_;
}

const C2S_Punch& C2S_Punch::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_punch_2eproto();
  return *default_instance_;
}

C2S_Punch* C2S_Punch::default_instance_ = NULL;

C2S_Punch* C2S_Punch::New() const {
  return new C2S_Punch;
}

void C2S_Punch::Clear() {
#define OFFSET_OF_FIELD_(f) (reinterpret_cast<char*>(      \
  &reinterpret_cast<C2S_Punch*>(16)->f) - \
   reinterpret_cast<char*>(16))

#define ZR_(first, last) do {                              \
    size_t f = OFFSET_OF_FIELD_(first);                    \
    size_t n = OFFSET_OF_FIELD_(last) - f + sizeof(last);  \
    ::memset(&first, 0, n);                                \
  } while (0)

  ZR_(from_, to_);

#undef OFFSET_OF_FIELD_
#undef ZR_

  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool C2S_Punch::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:SkyDream.C2S_Punch)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 from = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &from_)));
          set_has_from();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(16)) goto parse_to;
        break;
      }

      // required int32 to = 2;
      case 2: {
        if (tag == 16) {
         parse_to:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &to_)));
          set_has_to();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:SkyDream.C2S_Punch)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:SkyDream.C2S_Punch)
  return false;
#undef DO_
}

void C2S_Punch::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:SkyDream.C2S_Punch)
  // required int32 from = 1;
  if (has_from()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->from(), output);
  }

  // required int32 to = 2;
  if (has_to()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->to(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:SkyDream.C2S_Punch)
}

::google::protobuf::uint8* C2S_Punch::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:SkyDream.C2S_Punch)
  // required int32 from = 1;
  if (has_from()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->from(), target);
  }

  // required int32 to = 2;
  if (has_to()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->to(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:SkyDream.C2S_Punch)
  return target;
}

int C2S_Punch::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 from = 1;
    if (has_from()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->from());
    }

    // required int32 to = 2;
    if (has_to()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->to());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void C2S_Punch::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const C2S_Punch* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const C2S_Punch*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void C2S_Punch::MergeFrom(const C2S_Punch& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_from()) {
      set_from(from.from());
    }
    if (from.has_to()) {
      set_to(from.to());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void C2S_Punch::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void C2S_Punch::CopyFrom(const C2S_Punch& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool C2S_Punch::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void C2S_Punch::Swap(C2S_Punch* other) {
  if (other != this) {
    std::swap(from_, other->from_);
    std::swap(to_, other->to_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata C2S_Punch::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = C2S_Punch_descriptor_;
  metadata.reflection = C2S_Punch_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int IntValue::kValueFieldNumber;
#endif  // !_MSC_VER

IntValue::IntValue()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:SkyDream.IntValue)
}

void IntValue::InitAsDefaultInstance() {
}

IntValue::IntValue(const IntValue& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:SkyDream.IntValue)
}

void IntValue::SharedCtor() {
  _cached_size_ = 0;
  value_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

IntValue::~IntValue() {
  // @@protoc_insertion_point(destructor:SkyDream.IntValue)
  SharedDtor();
}

void IntValue::SharedDtor() {
  if (this != default_instance_) {
  }
}

void IntValue::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* IntValue::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return IntValue_descriptor_;
}

const IntValue& IntValue::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_punch_2eproto();
  return *default_instance_;
}

IntValue* IntValue::default_instance_ = NULL;

IntValue* IntValue::New() const {
  return new IntValue;
}

void IntValue::Clear() {
  value_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool IntValue::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:SkyDream.IntValue)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 value = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &value_)));
          set_has_value();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:SkyDream.IntValue)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:SkyDream.IntValue)
  return false;
#undef DO_
}

void IntValue::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:SkyDream.IntValue)
  // required int32 value = 1;
  if (has_value()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->value(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:SkyDream.IntValue)
}

::google::protobuf::uint8* IntValue::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:SkyDream.IntValue)
  // required int32 value = 1;
  if (has_value()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->value(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:SkyDream.IntValue)
  return target;
}

int IntValue::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 value = 1;
    if (has_value()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->value());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void IntValue::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const IntValue* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const IntValue*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void IntValue::MergeFrom(const IntValue& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_value()) {
      set_value(from.value());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void IntValue::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void IntValue::CopyFrom(const IntValue& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool IntValue::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void IntValue::Swap(IntValue* other) {
  if (other != this) {
    std::swap(value_, other->value_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata IntValue::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = IntValue_descriptor_;
  metadata.reflection = IntValue_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace SkyDream

// @@protoc_insertion_point(global_scope)
