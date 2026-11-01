#include "Tab.h"
#include "Category.h"
#include "Switch.h"
#include "FloatingSwitch.h"

class Widget {

private:
    JNIEnv* env;
    Tab tab;
    Category category;
    Switch aSwitch;
    FloatingSwitch aFloatingSwitch;

public:
    Widget(JNIEnv* globEnv) {
        env = globEnv;
    }

    void Tab(const char* name) {
        tab.create(env, name);
    }

    void Category(const char* name) {
        category.create(env, name);
    }

    void Switch(const char* name, jint ID) {
        aSwitch.create(env, name, ID);
    }

    void FloatingSwitch(const char* name, jint ID) {
        aFloatingSwitch.create(env, name, ID);
    }

    void FloatingSwitch(const char* name, const char* floatLabel, jint ID) {
        aFloatingSwitch.createWithLabel(env, name, floatLabel, ID);
    }

};
