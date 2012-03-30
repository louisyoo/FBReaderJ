/*
 * Copyright (C) 2011-2012 Geometer Plus <contact@geometerplus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <ZLLogger.h>

#include "JniEnvelope.h"

static const std::string JNI_LOGGER_CLASS = "JniLog";

JavaClass::JavaClass(JNIEnv *env, const std::string &name) : myName(name), myEnv(env) {
	jclass ref = env->FindClass(name.c_str());
	myClass = (jclass)env->NewGlobalRef(ref);
	env->DeleteLocalRef(ref);
}

JavaClass::~JavaClass() {
	myEnv->DeleteGlobalRef(myClass);
}

Member::Member(const JavaClass &cls) : myClass(cls) {
	//ZLLogger::Instance().registerClass(JNI_LOGGER_CLASS);
}

Member::~Member() {
}

Constructor::Constructor(const JavaClass &cls, const std::string &signature) : Member(cls) {
	myId = env().GetMethodID(jClass(), "<init>", signature.c_str());
}

jobject Constructor::call(...) {
	va_list lst;
	va_start(lst, this);
	jobject obj = env().NewObjectV(jClass(), myId, lst);
	va_end(lst);
	return obj;
}

Field::Field(const JavaClass &cls, const std::string &name, const std::string &type) : Member(cls), myName(name) {
	myId = env().GetFieldID(jClass(), name.c_str(), type.c_str());
}

Field::~Field() {
}

Method::Method(const JavaClass &cls, const std::string &name, const std::string &signature) : Member(cls), myName(name) {
	myId = env().GetMethodID(jClass(), name.c_str(), signature.c_str());
}

Method::~Method() {
}

StaticMethod::StaticMethod(const JavaClass &cls, const std::string &name, const std::string &signature) : Member(cls), myName(name) {
	myId = env().GetStaticMethodID(jClass(), name.c_str(), signature.c_str());
}

StaticMethod::~StaticMethod() {
}

ObjectField::ObjectField(const JavaClass &cls, const std::string &name, const std::string &type) : Field(cls, name, "L" + type + ";") {
}

jobject ObjectField::value(jobject obj) const {
	ZLLogger::Instance().println(JNI_LOGGER_CLASS, "getting value of ObjectField " + myName);
	jobject val = env().GetObjectField(obj, myId);
	ZLLogger::Instance().println(JNI_LOGGER_CLASS, "got value of ObjectField " + myName);
	return val;
}

VoidMethod::VoidMethod(const JavaClass &cls, const std::string &name, const std::string &signature) : Method(cls, name, signature + "V") {
}

void VoidMethod::call(jobject base, ...) {
	ZLLogger::Instance().println(JNI_LOGGER_CLASS, "calling VoidMethod " + myName);
	va_list lst;
	va_start(lst, base);
	env().CallVoidMethodV(base, myId, lst);
	va_end(lst);
	ZLLogger::Instance().println(JNI_LOGGER_CLASS, "finished VoidMethod " + myName);
}

IntMethod::IntMethod(const JavaClass &cls, const std::string &name, const std::string &signature) : Method(cls, name, signature + "I") {
}

jint IntMethod::call(jobject base, ...) {
	ZLLogger::Instance().println(JNI_LOGGER_CLASS, "calling IntMethod " + myName);
	va_list lst;
	va_start(lst, base);
	jint result = env().CallIntMethodV(base, myId, lst);
	va_end(lst);
	ZLLogger::Instance().println(JNI_LOGGER_CLASS, "finished IntMethod " + myName);
	return result;
}

LongMethod::LongMethod(const JavaClass &cls, const std::string &name, const std::string &signature) : Method(cls, name, signature + "J") {
}

jlong LongMethod::call(jobject base, ...) {
	ZLLogger::Instance().println(JNI_LOGGER_CLASS, "calling LongMethod " + myName);
	va_list lst;
	va_start(lst, base);
	jlong result = env().CallLongMethodV(base, myId, lst);
	va_end(lst);
	ZLLogger::Instance().println(JNI_LOGGER_CLASS, "finished LongMethod " + myName);
	return result;
}

BooleanMethod::BooleanMethod(const JavaClass &cls, const std::string &name, const std::string &signature) : Method(cls, name, signature + "Z") {
}

jboolean BooleanMethod::call(jobject base, ...) {
	ZLLogger::Instance().println(JNI_LOGGER_CLASS, "calling BooleanMethod " + myName);
	va_list lst;
	va_start(lst, base);
	jboolean result = env().CallBooleanMethodV(base, myId, lst);
	va_end(lst);
	ZLLogger::Instance().println(JNI_LOGGER_CLASS, "finished BooleanMethod " + myName);
	return result;
}

StringMethod::StringMethod(const JavaClass &cls, const std::string &name, const std::string &signature) : Method(cls, name, signature + "Ljava/lang/String;") {
}

jstring StringMethod::call(jobject base, ...) {
	ZLLogger::Instance().println(JNI_LOGGER_CLASS, "calling StringMethod " + myName);
	va_list lst;
	va_start(lst, base);
	jstring result = (jstring)env().CallObjectMethodV(base, myId, lst);
	va_end(lst);
	ZLLogger::Instance().println(JNI_LOGGER_CLASS, "finished StringMethod " + myName);
	return result;
}

ObjectMethod::ObjectMethod(const JavaClass &cls, const std::string &name, const std::string &returnType, const std::string &signature) : Method(cls, name, signature + "L" + returnType + ";") {
}

jobject ObjectMethod::call(jobject base, ...) {
	ZLLogger::Instance().println(JNI_LOGGER_CLASS, "calling ObjectMethod " + myName);
	va_list lst;
	va_start(lst, base);
	jobject result = env().CallObjectMethodV(base, myId, lst);
	va_end(lst);
	ZLLogger::Instance().println(JNI_LOGGER_CLASS, "finished ObjectMethod " + myName);
	return result;
}

StaticObjectMethod::StaticObjectMethod(const JavaClass &cls, const std::string &name, const std::string &returnType, const std::string &signature) : StaticMethod(cls, name, signature + "L" + returnType + ";") {
}

jobject StaticObjectMethod::call(...) {
	ZLLogger::Instance().println(JNI_LOGGER_CLASS, "calling StaticObjectMethod " + myName);
	va_list lst;
	va_start(lst, this);
	jobject result = env().CallStaticObjectMethodV(jClass(), myId, lst);
	va_end(lst);
	ZLLogger::Instance().println(JNI_LOGGER_CLASS, "finished StaticObjectMethod " + myName);
	return result;
}
