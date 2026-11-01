class FloatingSwitch {

public:
    void create(JNIEnv* env, const char* name, jint featureId) {
        createWithLabel(env, name, name, featureId);
    }

    void createWithLabel(JNIEnv* env, const char* menuName, const char* floatLabel, jint featureId) {
        jclass CMenu = env->FindClass("com/kanishkcheat/Menu");
        jmethodID MMethod = env->GetStaticMethodID(CMenu, "addFloatingCheckbox", "(Ljava/lang/String;IILjava/lang/String;)V");
        jint showId = featureId + 50000;
        env->CallStaticVoidMethod(CMenu, MMethod,
            env->NewStringUTF(menuName), showId, featureId, env->NewStringUTF(floatLabel));
    }

};
