// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: punch.proto

#ifndef PROTOBUF_punch_2eproto__INCLUDED
#define PROTOBUF_punch_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace SkyDream {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_punch_2eproto();
void protobuf_AssignDesc_punch_2eproto();
void protobuf_ShutdownFile_punch_2eproto();

class Person;
class ListConn;
class C2S_Punch;
class IntValue;

// ===================================================================

class Person : public ::google::protobuf::Message {
 public:
  Person();
  virtual ~Person();

  Person(const Person& from);

  inline Person& operator=(const Person& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Person& default_instance();

  void Swap(Person* other);

  // implements Message ----------------------------------------------

  Person* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Person& from);
  void MergeFrom(const Person& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string ip = 1;
  inline bool has_ip() const;
  inline void clear_ip();
  static const int kIpFieldNumber = 1;
  inline const ::std::string& ip() const;
  inline void set_ip(const ::std::string& value);
  inline void set_ip(const char* value);
  inline void set_ip(const char* value, size_t size);
  inline ::std::string* mutable_ip();
  inline ::std::string* release_ip();
  inline void set_allocated_ip(::std::string* ip);

  // required int32 port = 2;
  inline bool has_port() const;
  inline void clear_port();
  static const int kPortFieldNumber = 2;
  inline ::google::protobuf::int32 port() const;
  inline void set_port(::google::protobuf::int32 value);

  // required int32 clientId = 3;
  inline bool has_clientid() const;
  inline void clear_clientid();
  static const int kClientIdFieldNumber = 3;
  inline ::google::protobuf::int32 clientid() const;
  inline void set_clientid(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:SkyDream.Person)
 private:
  inline void set_has_ip();
  inline void clear_has_ip();
  inline void set_has_port();
  inline void clear_has_port();
  inline void set_has_clientid();
  inline void clear_has_clientid();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* ip_;
  ::google::protobuf::int32 port_;
  ::google::protobuf::int32 clientid_;
  friend void  protobuf_AddDesc_punch_2eproto();
  friend void protobuf_AssignDesc_punch_2eproto();
  friend void protobuf_ShutdownFile_punch_2eproto();

  void InitAsDefaultInstance();
  static Person* default_instance_;
};
// -------------------------------------------------------------------

class ListConn : public ::google::protobuf::Message {
 public:
  ListConn();
  virtual ~ListConn();

  ListConn(const ListConn& from);

  inline ListConn& operator=(const ListConn& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ListConn& default_instance();

  void Swap(ListConn* other);

  // implements Message ----------------------------------------------

  ListConn* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ListConn& from);
  void MergeFrom(const ListConn& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .SkyDream.Person persons = 1;
  inline int persons_size() const;
  inline void clear_persons();
  static const int kPersonsFieldNumber = 1;
  inline const ::SkyDream::Person& persons(int index) const;
  inline ::SkyDream::Person* mutable_persons(int index);
  inline ::SkyDream::Person* add_persons();
  inline const ::google::protobuf::RepeatedPtrField< ::SkyDream::Person >&
      persons() const;
  inline ::google::protobuf::RepeatedPtrField< ::SkyDream::Person >*
      mutable_persons();

  // @@protoc_insertion_point(class_scope:SkyDream.ListConn)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::RepeatedPtrField< ::SkyDream::Person > persons_;
  friend void  protobuf_AddDesc_punch_2eproto();
  friend void protobuf_AssignDesc_punch_2eproto();
  friend void protobuf_ShutdownFile_punch_2eproto();

  void InitAsDefaultInstance();
  static ListConn* default_instance_;
};
// -------------------------------------------------------------------

class C2S_Punch : public ::google::protobuf::Message {
 public:
  C2S_Punch();
  virtual ~C2S_Punch();

  C2S_Punch(const C2S_Punch& from);

  inline C2S_Punch& operator=(const C2S_Punch& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const C2S_Punch& default_instance();

  void Swap(C2S_Punch* other);

  // implements Message ----------------------------------------------

  C2S_Punch* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const C2S_Punch& from);
  void MergeFrom(const C2S_Punch& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 from = 1;
  inline bool has_from() const;
  inline void clear_from();
  static const int kFromFieldNumber = 1;
  inline ::google::protobuf::int32 from() const;
  inline void set_from(::google::protobuf::int32 value);

  // required int32 to = 2;
  inline bool has_to() const;
  inline void clear_to();
  static const int kToFieldNumber = 2;
  inline ::google::protobuf::int32 to() const;
  inline void set_to(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:SkyDream.C2S_Punch)
 private:
  inline void set_has_from();
  inline void clear_has_from();
  inline void set_has_to();
  inline void clear_has_to();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 from_;
  ::google::protobuf::int32 to_;
  friend void  protobuf_AddDesc_punch_2eproto();
  friend void protobuf_AssignDesc_punch_2eproto();
  friend void protobuf_ShutdownFile_punch_2eproto();

  void InitAsDefaultInstance();
  static C2S_Punch* default_instance_;
};
// -------------------------------------------------------------------

class IntValue : public ::google::protobuf::Message {
 public:
  IntValue();
  virtual ~IntValue();

  IntValue(const IntValue& from);

  inline IntValue& operator=(const IntValue& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const IntValue& default_instance();

  void Swap(IntValue* other);

  // implements Message ----------------------------------------------

  IntValue* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const IntValue& from);
  void MergeFrom(const IntValue& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 value = 1;
  inline bool has_value() const;
  inline void clear_value();
  static const int kValueFieldNumber = 1;
  inline ::google::protobuf::int32 value() const;
  inline void set_value(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:SkyDream.IntValue)
 private:
  inline void set_has_value();
  inline void clear_has_value();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 value_;
  friend void  protobuf_AddDesc_punch_2eproto();
  friend void protobuf_AssignDesc_punch_2eproto();
  friend void protobuf_ShutdownFile_punch_2eproto();

  void InitAsDefaultInstance();
  static IntValue* default_instance_;
};
// ===================================================================


// ===================================================================

// Person

// required string ip = 1;
inline bool Person::has_ip() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Person::set_has_ip() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Person::clear_has_ip() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Person::clear_ip() {
  if (ip_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    ip_->clear();
  }
  clear_has_ip();
}
inline const ::std::string& Person::ip() const {
  // @@protoc_insertion_point(field_get:SkyDream.Person.ip)
  return *ip_;
}
inline void Person::set_ip(const ::std::string& value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
  // @@protoc_insertion_point(field_set:SkyDream.Person.ip)
}
inline void Person::set_ip(const char* value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
  // @@protoc_insertion_point(field_set_char:SkyDream.Person.ip)
}
inline void Person::set_ip(const char* value, size_t size) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    ip_ = new ::std::string;
  }
  ip_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:SkyDream.Person.ip)
}
inline ::std::string* Person::mutable_ip() {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    ip_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:SkyDream.Person.ip)
  return ip_;
}
inline ::std::string* Person::release_ip() {
  clear_has_ip();
  if (ip_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = ip_;
    ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void Person::set_allocated_ip(::std::string* ip) {
  if (ip_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete ip_;
  }
  if (ip) {
    set_has_ip();
    ip_ = ip;
  } else {
    clear_has_ip();
    ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:SkyDream.Person.ip)
}

// required int32 port = 2;
inline bool Person::has_port() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Person::set_has_port() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Person::clear_has_port() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Person::clear_port() {
  port_ = 0;
  clear_has_port();
}
inline ::google::protobuf::int32 Person::port() const {
  // @@protoc_insertion_point(field_get:SkyDream.Person.port)
  return port_;
}
inline void Person::set_port(::google::protobuf::int32 value) {
  set_has_port();
  port_ = value;
  // @@protoc_insertion_point(field_set:SkyDream.Person.port)
}

// required int32 clientId = 3;
inline bool Person::has_clientid() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Person::set_has_clientid() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Person::clear_has_clientid() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Person::clear_clientid() {
  clientid_ = 0;
  clear_has_clientid();
}
inline ::google::protobuf::int32 Person::clientid() const {
  // @@protoc_insertion_point(field_get:SkyDream.Person.clientId)
  return clientid_;
}
inline void Person::set_clientid(::google::protobuf::int32 value) {
  set_has_clientid();
  clientid_ = value;
  // @@protoc_insertion_point(field_set:SkyDream.Person.clientId)
}

// -------------------------------------------------------------------

// ListConn

// repeated .SkyDream.Person persons = 1;
inline int ListConn::persons_size() const {
  return persons_.size();
}
inline void ListConn::clear_persons() {
  persons_.Clear();
}
inline const ::SkyDream::Person& ListConn::persons(int index) const {
  // @@protoc_insertion_point(field_get:SkyDream.ListConn.persons)
  return persons_.Get(index);
}
inline ::SkyDream::Person* ListConn::mutable_persons(int index) {
  // @@protoc_insertion_point(field_mutable:SkyDream.ListConn.persons)
  return persons_.Mutable(index);
}
inline ::SkyDream::Person* ListConn::add_persons() {
  // @@protoc_insertion_point(field_add:SkyDream.ListConn.persons)
  return persons_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::SkyDream::Person >&
ListConn::persons() const {
  // @@protoc_insertion_point(field_list:SkyDream.ListConn.persons)
  return persons_;
}
inline ::google::protobuf::RepeatedPtrField< ::SkyDream::Person >*
ListConn::mutable_persons() {
  // @@protoc_insertion_point(field_mutable_list:SkyDream.ListConn.persons)
  return &persons_;
}

// -------------------------------------------------------------------

// C2S_Punch

// required int32 from = 1;
inline bool C2S_Punch::has_from() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void C2S_Punch::set_has_from() {
  _has_bits_[0] |= 0x00000001u;
}
inline void C2S_Punch::clear_has_from() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void C2S_Punch::clear_from() {
  from_ = 0;
  clear_has_from();
}
inline ::google::protobuf::int32 C2S_Punch::from() const {
  // @@protoc_insertion_point(field_get:SkyDream.C2S_Punch.from)
  return from_;
}
inline void C2S_Punch::set_from(::google::protobuf::int32 value) {
  set_has_from();
  from_ = value;
  // @@protoc_insertion_point(field_set:SkyDream.C2S_Punch.from)
}

// required int32 to = 2;
inline bool C2S_Punch::has_to() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void C2S_Punch::set_has_to() {
  _has_bits_[0] |= 0x00000002u;
}
inline void C2S_Punch::clear_has_to() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void C2S_Punch::clear_to() {
  to_ = 0;
  clear_has_to();
}
inline ::google::protobuf::int32 C2S_Punch::to() const {
  // @@protoc_insertion_point(field_get:SkyDream.C2S_Punch.to)
  return to_;
}
inline void C2S_Punch::set_to(::google::protobuf::int32 value) {
  set_has_to();
  to_ = value;
  // @@protoc_insertion_point(field_set:SkyDream.C2S_Punch.to)
}

// -------------------------------------------------------------------

// IntValue

// required int32 value = 1;
inline bool IntValue::has_value() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void IntValue::set_has_value() {
  _has_bits_[0] |= 0x00000001u;
}
inline void IntValue::clear_has_value() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void IntValue::clear_value() {
  value_ = 0;
  clear_has_value();
}
inline ::google::protobuf::int32 IntValue::value() const {
  // @@protoc_insertion_point(field_get:SkyDream.IntValue.value)
  return value_;
}
inline void IntValue::set_value(::google::protobuf::int32 value) {
  set_has_value();
  value_ = value;
  // @@protoc_insertion_point(field_set:SkyDream.IntValue.value)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace SkyDream

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_punch_2eproto__INCLUDED