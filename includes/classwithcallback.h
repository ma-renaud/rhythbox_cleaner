#ifndef RHYTHBOX_CLEANER_CLASSWITHCALLBACK_H
#define RHYTHBOX_CLEANER_CLASSWITHCALLBACK_H

#include "rhythmdbxml.h"

typedef void (*LPFN_SaxCCallback)(mxml_node_t *, mxml_sax_event_t, void *);
typedef void (RhythmDBXML::*LPFN_SaxMemberFunctionCallback)(mxml_node_t *, mxml_sax_event_t, void *);

// this object holds the state for a C++ member function callback in memory
class SaxCallbackBase {
 public:
  // input: pointer to a unique C callback.
  explicit SaxCallbackBase(LPFN_SaxCCallback pCCallback) : m_pClass(nullptr), m_pMethod(nullptr), m_pCCallback(pCCallback) { }

  // when done, remove allocation of the callback
  void Free() {
    m_pClass = nullptr;
    // not clearing m_pMethod: it won't be used, since m_pClass is nullptr and so this entry is marked as free
  }

  // when free, allocate this callback
  LPFN_SaxCCallback Reserve(RhythmDBXML *instance, LPFN_SaxMemberFunctionCallback method) {
    if (m_pClass)
      return nullptr;

    m_pClass = instance;
    m_pMethod = method;
    return m_pCCallback;
  }

 protected:
  static void StaticInvoke(int context, mxml_node_t *node, mxml_sax_event_t event, void *data);

 private:
  RhythmDBXML *m_pClass;
  LPFN_SaxMemberFunctionCallback m_pMethod;
  LPFN_SaxCCallback m_pCCallback;
};

template<int context>
class DynamicSaxCallback : public SaxCallbackBase {
 public:
  DynamicSaxCallback()
      : SaxCallbackBase(&DynamicSaxCallback<context>::GeneratedStaticFunction) {
  }

 private:
  static void GeneratedStaticFunction(mxml_node_t *node, mxml_sax_event_t event, void *data) {
    return StaticInvoke(context, node, event, data);
  }
};

class SaxMemberFunctionCallback {
 public:
  SaxMemberFunctionCallback(RhythmDBXML *instance, LPFN_SaxMemberFunctionCallback method);
  ~SaxMemberFunctionCallback();

  SaxMemberFunctionCallback(const SaxMemberFunctionCallback &os) = delete;
  SaxMemberFunctionCallback &operator=(const SaxMemberFunctionCallback &os) = delete;

 public:
  operator LPFN_SaxCCallback() const {
    return m_cbCallback;
  }

  bool IsValid() const {
    return m_cbCallback != nullptr;
  }

 private:
  //DynamicSaxCallback<0x00> dynamic_sax_callback;
  LPFN_SaxCCallback m_cbCallback;
  int m_nAllocIndex;
};

static SaxCallbackBase *AvailableCallbackSlots[] = {
    new DynamicSaxCallback<0x00>(),
    new DynamicSaxCallback<0x01>(),
    new DynamicSaxCallback<0x02>(),
    new DynamicSaxCallback<0x03>(),
    new DynamicSaxCallback<0x04>(),
    new DynamicSaxCallback<0x05>(),
    new DynamicSaxCallback<0x06>(),
    new DynamicSaxCallback<0x07>(),
    new DynamicSaxCallback<0x08>(),
    new DynamicSaxCallback<0x09>(),
    new DynamicSaxCallback<0x0A>(),
    new DynamicSaxCallback<0x0B>(),
    new DynamicSaxCallback<0x0C>(),
    new DynamicSaxCallback<0x0D>(),
    new DynamicSaxCallback<0x0E>(),
    new DynamicSaxCallback<0x0F>(),
};

void SaxCallbackBase::StaticInvoke(int context, mxml_node_t *node, mxml_sax_event_t event, void *data) {
  return ((AvailableCallbackSlots[context]->m_pClass)->*(AvailableCallbackSlots[context]->m_pMethod))(node, event, data);
}

SaxMemberFunctionCallback::SaxMemberFunctionCallback(RhythmDBXML *instance, LPFN_SaxMemberFunctionCallback method) {
  int imax = sizeof(AvailableCallbackSlots) / sizeof(AvailableCallbackSlots[0]);
  for (m_nAllocIndex = 0; m_nAllocIndex < imax; ++m_nAllocIndex) {
    m_cbCallback = AvailableCallbackSlots[m_nAllocIndex]->Reserve(instance, method);
    if (m_cbCallback != nullptr)
      break;
  }

  //m_cbCallback = dynamic_sax_callback.Reserve(instance, method);
}

SaxMemberFunctionCallback::~SaxMemberFunctionCallback() {
  if (IsValid()) {
    AvailableCallbackSlots[m_nAllocIndex]->Free();
  }
}

#endif //RHYTHBOX_CLEANER_CLASSWITHCALLBACK_H
