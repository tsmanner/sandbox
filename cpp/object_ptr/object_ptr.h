#ifndef object_ptr_h
#define object_ptr_h


#include <functional>
#include <utility>


class object_ptr_interface {
public:
  object_ptr_interface(): mReferenceCount(0) {}
  void incrementReferenceCount() { ++mReferenceCount; }
  void decrementReferenceCount() { --mReferenceCount; }
  const int& getReferenceCount() { return mReferenceCount; }

private:
  int mReferenceCount;
};


template <typename ObjectType>
class object_ptr {
public:
  // Default
  object_ptr(): mObject(nullptr) {}
  // By ObjectType
  object_ptr(ObjectType* object): object_ptr() { set(object); }
  object_ptr(ObjectType& object): object_ptr() { set(&object); }
  // Copy
  object_ptr(object_ptr& other): object_ptr() { set(other.get()); }
  object_ptr(const object_ptr& other): object_ptr() { set(other.get()); }
  // Move
  object_ptr(object_ptr&& other): mObject(other.release()) {}

  virtual ~object_ptr() { reset(); }

  void set(ObjectType* object) { reset(object); }

  void reset(ObjectType* object=nullptr) {
    if (mObject) {
      mObject->object_ptr_interface::decrementReferenceCount();
      if (mObject->object_ptr_interface::getReferenceCount() == 0) delete mObject;
    }
    mObject = object;
    if (mObject) {
      mObject->object_ptr_interface::incrementReferenceCount();
    }
  }

  // release function used for two purposes:
  //   1 Move construction/assignment, so we don't have to copy
  //     the pointer and increment then immediately decrement.
  //   2 Initialization to guarantee that we don't wrap a garbage
  //     pointer and try to decrement it on the first use.
  ObjectType* release() {
    ObjectType* object = get();
    mObject = nullptr;
    return object;
  }

  ObjectType& operator*() const { return *get(); }
  ObjectType* operator->() const { return get(); }

  ObjectType* get() const { return mObject; }
  // Object Assignment
  void operator=(ObjectType* object) { set(object); }
  // Copy Assignment
  void operator=(const object_ptr<ObjectType>& ptr) { set(ptr.get()); }
  // Move Assignment
  void operator=(object_ptr<ObjectType>&& ptr) { set(ptr.release()); }

  bool operator==(const object_ptr<ObjectType>& rhs) const { return get() == rhs.get(); }
  bool operator<(const object_ptr<ObjectType>& rhs) const { return get() < rhs.get(); }
  explicit operator bool() { return (get() == nullptr) ? false : true; }

  explicit operator ObjectType*() { return get(); }

private:

  ObjectType* mObject;

};


//
// Define a hasher so object_ptr can take part in unordered collections.
//
namespace std {

  template<typename ObjectType>
  struct hash<object_ptr<ObjectType>>: public std::unary_function<object_ptr<ObjectType>, size_t> {
    size_t operator()(const object_ptr<ObjectType>& ptr) const { return std::hash<ObjectType*>()(ptr.get()); }
  };

}

#endif
