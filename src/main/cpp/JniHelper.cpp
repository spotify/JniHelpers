#include "JniHelper.h"

namespace spotify {

static JniHelper *gInstance = NULL;

JniHelper::JniHelper(JavaVM *jvm) {
    _jvm = jvm;
}

JniHelper::~JniHelper() {
}

void JniHelper::initialize(JavaVM *jvm) {
    gInstance = new JniHelper(jvm);
}

JniHelper* JniHelper::get() {
    return gInstance;
}

} // namespace spotify
