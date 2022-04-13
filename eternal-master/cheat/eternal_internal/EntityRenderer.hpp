#include <jni.h>
#include "functions.hpp"
#include "global.hpp"

class c_entityrenderer {

private:
    JNIEnv* env;
    jclass class_;

public:

    c_entityrenderer()
    {
        class_ = getObject(env, global::is_badlion ? "bfk" : "net.minecraft.client.renderer.EntityRenderer");
        env = global::jenv;
    }

    // rayTrace
    // getPositionEyes
    // getLook
    // ridingEntity 

};