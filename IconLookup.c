#include <jni.h>
#include "IconLookup.h"
#include "linicon_ffi.h"
#include <stdint.h>
#include <stdlib.h>

JNIEXPORT jobject JNICALL
Java_IconLookup_lookup(
    JNIEnv *env,
    jclass thisC,
    jstring theme,
    jstring icon_name,
    jint size,
    jint scale)
{
    char *theme_str = (char *)(*env)->GetStringUTFChars(env, theme, NULL);
    char *name_str = (char *)(*env)->GetStringUTFChars(env, icon_name, NULL);
    linicon_IconIter *iter =
        linicon_lookup_icon(theme_str, name_str, (uint16_t)size, (uint16_t)scale);
    if (iter == NULL && linicon_have_last_error())
    {
        int err_len = linicon_last_error_length();
        char *err_msg = malloc(err_len * sizeof(char));
        linicon_last_error_message(err_msg, err_len);
        jclass errC = (*env)->FindClass(env, "IconLookupException");
        (*env)->ThrowNew(env, errC, err_msg);
        free(err_msg);
        return NULL;
    }
    jobject newIter = (*env)->AllocObject(env, thisC);
    jfieldID ptrF = (*env)->GetFieldID(env, thisC, "iterPtr", "J");
    (*env)->SetLongField(env, newIter, ptrF, (long)iter);
    return newIter;
}

JNIEXPORT jobject JNICALL
Java_IconLookup_lookupWithExtraPaths(
    JNIEnv *env,
    jclass thisC,
    jstring theme,
    jstring icon_name,
    jint size,
    jint scale,
    jobjectArray extraPaths)
{
    char *theme_str = (char *)(*env)->GetStringUTFChars(env, theme, NULL);
    char *name_str = (char *)(*env)->GetStringUTFChars(env, icon_name, NULL);
    int pathsArrLen = (*env)->GetArrayLength(env, extraPaths);
    char **paths = malloc(pathsArrLen * sizeof(char *));
    for (int i = 0; i < pathsArrLen; i++)
    {
        jobject path = (*env)->GetObjectArrayElement(env, extraPaths, i);
        paths[i] = (char *)(*env)->GetStringUTFChars(env, path, NULL);
    }
    linicon_IconIter *iter =
        linicon_lookup_icon_with_extra_paths(theme_str, name_str,
                                             (uint16_t)size, (uint16_t)scale,
                                             paths, pathsArrLen);
    if (iter == NULL && linicon_have_last_error())
    {
        int err_len = linicon_last_error_length();
        char *err_msg = malloc(err_len * sizeof(char));
        linicon_last_error_message(err_msg, err_len);
        jclass errC = (*env)->FindClass(env, "IconLookupException");
        (*env)->ThrowNew(env, errC, err_msg);
        free(err_msg);
        return NULL;
    }
    jobject newIter = (*env)->AllocObject(env, thisC);
    jfieldID ptrF = (*env)->GetFieldID(env, thisC, "iterPtr", "J");
    (*env)->SetLongField(env, newIter, ptrF, (long)iter);
    return newIter;
}

JNIEXPORT jobject JNICALL
Java_IconLookup_nextIcon(
    JNIEnv *env,
    jobject thisO)
{
    jclass thisC = (*env)->GetObjectClass(env, thisO);
    jfieldID ptrF = (*env)->GetFieldID(env, thisC, "iterPtr", "J");
    linicon_IconIter *iter =
        (linicon_IconIter *)(*env)->GetLongField(env, thisO, ptrF);

    linicon_IconPath *iconPath = linicon_iter_next(iter);

    if (iconPath == NULL)
    {
        if (linicon_have_last_error())
        {
            int err_len = linicon_last_error_length();
            char *err_msg = malloc(err_len * sizeof(char));
            linicon_last_error_message(err_msg, err_len);
            jclass errC = (*env)->FindClass(env, "IconLookupException");
            (*env)->ThrowNew(env, errC, err_msg);
            free(err_msg);
        }
        return NULL;
    }

    jclass iconTypeC = (*env)->FindClass(env, "IconType");
    jfieldID iconTypeF;
    if (iconPath->icon_type == PNG)
    {
        iconTypeF = (*env)->GetStaticFieldID(env, iconTypeC, "PNG", "LIconType;");
    }
    else if (iconPath->icon_type == SVG)
    {
        iconTypeF = (*env)->GetStaticFieldID(env, iconTypeC, "SVG", "LIconType;");
    }
    else
    {
        iconTypeF = (*env)->GetStaticFieldID(env, iconTypeC, "XMP", "LIconType;");
    }
    jobject iconType = (*env)->GetStaticObjectField(env, iconTypeC, iconTypeF);

    jclass iconPathC = (*env)->FindClass(env, "IconPath");
    jobject iconPathO = (*env)->AllocObject(env, iconPathC);

    jfieldID themeF = (*env)->GetFieldID(env, iconPathC, "theme", "Ljava/lang/String;");
    jfieldID pathF = (*env)->GetFieldID(env, iconPathC, "path", "Ljava/lang/String;");
    jfieldID typeF = (*env)->GetFieldID(env, iconPathC, "iconType", "LIconType;");

    (*env)->SetObjectField(env, iconPathO, pathF, (*env)->NewStringUTF(env, iconPath->path));
    (*env)->SetObjectField(env, iconPathO, themeF, (*env)->NewStringUTF(env, iconPath->theme));
    (*env)->SetObjectField(env, iconPathO, typeF, iconType);

    linicon_free_icon_path(iconPath);

    return iconPathO;
}

JNIEXPORT void JNICALL
Java_IconLookup_finish(
    JNIEnv *env,
    jclass thisO)
{
    jclass thisC = (*env)->GetObjectClass(env, thisO);
    jfieldID ptrF = (*env)->GetFieldID(env, thisC, "iterPtr", "J");
    linicon_IconIter *ptr = (linicon_IconIter *)(*env)->GetLongField(env, thisO, ptrF);

    if (ptr != NULL)
    {
        linicon_free_icon_iter((linicon_IconIter *)ptr);
        (*env)->SetLongField(env, thisO, ptrF, (jlong)NULL);
    }
}