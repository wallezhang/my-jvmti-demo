//
// Created by IronMan on 2020/10/17.
//

#include <iostream>
#include "jvmti.h"

jint printLoadedClasses(JavaVM * vm);
void getEnv(JavaVM * vm);

void registerEvents();

void methodEntry(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jmethodID method);

jvmtiEnv *jvmti;

JNIEXPORT jint JNICALL
Agent_OnLoad(JavaVM * vm, char * options, void * reserved) {
    std::cout << "Agent OnLoad" << std::endl;
    getEnv(vm);
    registerEvents();
    return 0;
}

jint JNICALL
Agent_OnAttach(JavaVM * vm, char * options, void * reserved) {
    std::cout << "Agent OnAttach" << std::endl;
    getEnv(vm);
    registerEvents();
    return printLoadedClasses(vm);
}

void getEnv(JavaVM * vm) {
    jint result = vm->GetEnv((void **) &jvmti, JVMTI_VERSION_1_2);
    if (result != JNI_OK) {
        std::cout << "Unable to access jvm env, err code = " << result << std::endl;
    }

    auto capabilities = new jvmtiCapabilities;
    capabilities->can_generate_method_entry_events = 1;
    jvmtiError error = jvmti->AddCapabilities(capabilities);
    if (error != JVMTI_ERROR_NONE) {
        std::cout << "AddCapabilities failed, err = " << error << std::endl;
    }
}

JNIEXPORT void JNICALL
Agent_OnUnload(JavaVM * vm) {
    std::cout << "Agent OnUnload" << std::endl;
}

void registerEvents() {
    jvmtiError error = jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_METHOD_ENTRY, nullptr);
    if (error != JVMTI_ERROR_NONE) {
        std::cout << "SetEventNotificationMode failed, err = " << error << std::endl;
        return;
    }

    jvmtiEventCallbacks *callbacks;
    callbacks = new jvmtiEventCallbacks;
    callbacks->MethodEntry = &methodEntry;
    jvmti->SetEventCallbacks(callbacks, sizeof(*callbacks));
}

void methodEntry(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jmethodID method) {
    char *name;
    char *sig;
    jvmtiError error = jvmti_env->GetMethodName(method, &name, &sig, nullptr);
    if (error != JVMTI_ERROR_NONE) {
        std::cout << "GetMethodName error, code = " << error << std::endl;
        return;
    }

    std::cout << "Method " << name << sig << " called!" << std::endl;
}

JNIEXPORT jint printLoadedClasses(JavaVM * vm) {
    jvmtiEnv *jvmti;

    jint result = vm->GetEnv((void **) &jvmti, JVMTI_VERSION_1_2);
    if (result != JNI_OK) {
        std::cout << "Unable to access jvm env" << std::endl;
        return result;
    }

    jclass * classes;
    jint count;
    result = jvmti->GetLoadedClasses(&count, &classes);
    std::cout << "GetLoadedClasses result = " << result << std::endl;
    if (result != JNI_OK) {
        std::cout << "JVMTI GetLoadedClasses failed" << std::endl;
        return result;
    }

    for (int i = 0; i < count; i++) {
        char *sig;
        char *genericSig;
        jvmti->GetClassSignature(classes[i], &sig, &genericSig);
        std::cout << "class signature = " << sig << std::endl;
    }

    return 0;
}