/*
 * Copyright (c) 2014 Spotify AB
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "ClassRegistry.h"

namespace spotify {
namespace jni {

ClassRegistry::ClassRegistry() {
  LOG_DEBUG("Creating new class registry");
}

ClassRegistry::~ClassRegistry() {
  LOG_DEBUG("Destroying class registry");
}

void ClassRegistry::add(JNIEnv *env, const JavaClass *item) {
  if (item == NULL) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
      "Can't add null item to registry");
    return;
  } else if (item->getCanonicalName() == NULL || strlen(item->getCanonicalName()) == 0) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
      "Can't add item with empty canonical name to registry");
  } else if (!item->isInitialized()) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
      "Can't add uninitialized JavaClass to registry");
  } else {
    LOG_INFO("Adding class instance '%s' to registry", item->getCanonicalName());
    _classes[item->getCanonicalName()].set(item);
  }
}

const JavaClass* ClassRegistry::get(const char* name) const {
  LOG_DEBUG("Looking up class named '%s'", name);
  if (name == NULL) {
    JNIEnv *env = JavaThreadUtils::getEnvForCurrentThread();
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
      "Can't call get() with NULL");
    return NULL;
  }

  const ClassRegistryMap::const_iterator iter = _classes.find(name);
  return iter != _classes.end() ? iter->second.get() : NULL;
}

} // namespace jni
} // namespace spotify
