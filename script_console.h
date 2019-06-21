#pragma once

#include <iostream>
#include <angelscript.h> // http://www.angelcode.com/angelscript/sdk/docs/manual/doc_compile_lib.html
#include <chrono>
#include <thread>
#include <string>
#include <fstream>
#include <sstream>
#include "graph.h"

#include "angelscript\scriptarray\scriptarray.h"
#include "angelscript\scriptfile\scriptfile.h"
#include "angelscript\scriptfile\scriptfilesystem.h"
#include "angelscript\scriptmath\scriptmath.h"
#include "angelscript\scriptstdstring\scriptstdstring.h"
#include "angelscript\stl_containers\aatc.hpp"

class script_console
	{
	void RegisterScriptPrint(asIScriptEngine* engine);
	void RegisterScriptGraph(asIScriptEngine* engine);

	std::chrono::system_clock::time_point timeout_base;
	std::chrono::milliseconds timeout_duration;

	graph* g;
	asIScriptEngine* engine;
	asIScriptContext *ctx;
	
	static void MessageCallback(const asSMessageInfo *msg, void *param);
	static void LineCallback(asIScriptContext *ctx, std::chrono::system_clock::time_point *timeout_base);
	void configure_engine();
	bool compile(std::string &script);
	bool run(std::string &script);

	void help();

	public:
		script_console(bool &success, graph* g);
		~script_console();
		bool open();
		bool load(std::string &fname);

	};

