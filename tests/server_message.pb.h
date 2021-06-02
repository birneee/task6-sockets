// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: server_message.proto

#ifndef PROTOBUF_server_5fmessage_2eproto__INCLUDED
#define PROTOBUF_server_5fmessage_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace sockets {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_server_5fmessage_2eproto();
void protobuf_AssignDesc_server_5fmessage_2eproto();
void protobuf_ShutdownFile_server_5fmessage_2eproto();

class server_msg;

// ===================================================================

class server_msg : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:sockets.server_msg) */ {
 public:
  server_msg();
  virtual ~server_msg();

  server_msg(const server_msg& from);

  inline server_msg& operator=(const server_msg& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const server_msg& default_instance();

  void Swap(server_msg* other);

  // implements Message ----------------------------------------------

  inline server_msg* New() const { return New(NULL); }

  server_msg* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const server_msg& from);
  void MergeFrom(const server_msg& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(server_msg* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 result = 1;
  bool has_result() const;
  void clear_result();
  static const int kResultFieldNumber = 1;
  ::google::protobuf::int32 result() const;
  void set_result(::google::protobuf::int32 value);

  // required int64 total_bytes = 2;
  bool has_total_bytes() const;
  void clear_total_bytes();
  static const int kTotalBytesFieldNumber = 2;
  ::google::protobuf::int64 total_bytes() const;
  void set_total_bytes(::google::protobuf::int64 value);

  // @@protoc_insertion_point(class_scope:sockets.server_msg)
 private:
  inline void set_has_result();
  inline void clear_has_result();
  inline void set_has_total_bytes();
  inline void clear_has_total_bytes();

  // helper for ByteSize()
  int RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int64 total_bytes_;
  ::google::protobuf::int32 result_;
  friend void  protobuf_AddDesc_server_5fmessage_2eproto();
  friend void protobuf_AssignDesc_server_5fmessage_2eproto();
  friend void protobuf_ShutdownFile_server_5fmessage_2eproto();

  void InitAsDefaultInstance();
  static server_msg* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// server_msg

// required int32 result = 1;
inline bool server_msg::has_result() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void server_msg::set_has_result() {
  _has_bits_[0] |= 0x00000001u;
}
inline void server_msg::clear_has_result() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void server_msg::clear_result() {
  result_ = 0;
  clear_has_result();
}
inline ::google::protobuf::int32 server_msg::result() const {
  // @@protoc_insertion_point(field_get:sockets.server_msg.result)
  return result_;
}
inline void server_msg::set_result(::google::protobuf::int32 value) {
  set_has_result();
  result_ = value;
  // @@protoc_insertion_point(field_set:sockets.server_msg.result)
}

// required int64 total_bytes = 2;
inline bool server_msg::has_total_bytes() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void server_msg::set_has_total_bytes() {
  _has_bits_[0] |= 0x00000002u;
}
inline void server_msg::clear_has_total_bytes() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void server_msg::clear_total_bytes() {
  total_bytes_ = GOOGLE_LONGLONG(0);
  clear_has_total_bytes();
}
inline ::google::protobuf::int64 server_msg::total_bytes() const {
  // @@protoc_insertion_point(field_get:sockets.server_msg.total_bytes)
  return total_bytes_;
}
inline void server_msg::set_total_bytes(::google::protobuf::int64 value) {
  set_has_total_bytes();
  total_bytes_ = value;
  // @@protoc_insertion_point(field_set:sockets.server_msg.total_bytes)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace sockets

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_server_5fmessage_2eproto__INCLUDED
