#include "stdafx.h"
#include "script_console.h"

void print(std::string &s) { std::cout << s << std::endl; }
void sleep(unsigned int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }


void script_console::RegisterScriptPrint(asIScriptEngine * engine)
	{
	engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL);
	engine->RegisterGlobalFunction("void sleep(const uint ms)", asFUNCTION(sleep), asCALL_CDECL);
	}

void script_console::RegisterScriptGraph(asIScriptEngine * engine)
	{
	engine->RegisterObjectType("graph", 0, asOBJ_REF);
	engine->RegisterObjectType("node", 0, asOBJ_REF);
	engine->RegisterObjectType("arc", 0, asOBJ_REF);

	engine->RegisterObjectBehaviour("graph", asBEHAVE_ADDREF, "void f()", asMETHOD(graph, AddRef), asCALL_THISCALL);
	engine->RegisterObjectBehaviour("graph", asBEHAVE_RELEASE, "void f()", asMETHOD(graph, ReleaseRef), asCALL_THISCALL);
	engine->RegisterObjectBehaviour("node", asBEHAVE_ADDREF, "void f()", asMETHOD(graph::node, AddRef), asCALL_THISCALL);
	engine->RegisterObjectBehaviour("node", asBEHAVE_RELEASE, "void f()", asMETHOD(graph::node, ReleaseRef), asCALL_THISCALL);
	engine->RegisterObjectBehaviour("arc", asBEHAVE_ADDREF, "void f()", asMETHOD(graph::arc, AddRef), asCALL_THISCALL);
	engine->RegisterObjectBehaviour("arc", asBEHAVE_RELEASE, "void f()", asMETHOD(graph::arc, ReleaseRef), asCALL_THISCALL);

	engine->RegisterObjectMethod("graph", "node@ node_next()", asMETHOD(graph, node_next), asCALL_THISCALL);
	engine->RegisterObjectMethod("graph", "node@ node_reset()", asMETHOD(graph, node_reset), asCALL_THISCALL);
	engine->RegisterObjectMethod("graph", "node@ add_node()", asMETHOD(graph, add_node), asCALL_THISCALL);
	engine->RegisterObjectMethod("graph", "bool remove_node(node@ n)", asMETHOD(graph, remove_node), asCALL_THISCALL);
	engine->RegisterObjectMethod("graph", "arc@ add_arc(node@ from, node@ to, bool directed = false, bool merge = true)", asMETHOD(graph, add_arc), asCALL_THISCALL);
	engine->RegisterObjectMethod("graph", "bool remove_arc(arc@ n)", asMETHOD(graph, remove_arc), asCALL_THISCALL);

	engine->RegisterObjectMethod("graph", "bool shr_chk(string var_name)", asMETHOD(graph, shared_nodes_variable_chk), asCALL_THISCALL);
	engine->RegisterObjectMethod("graph", "void shr_set(string var_name, double def = 0)", asMETHOD(graph, shared_nodes_variable_set), asCALL_THISCALL);
	engine->RegisterObjectMethod("graph", "void shr_del(string var_name)", asMETHOD(graph, shared_nodes_variable_del), asCALL_THISCALL);
	engine->RegisterObjectMethod("graph", "bool shr_arc_chk(string var_name)", asMETHOD(graph, shared_arcs_variable_chk), asCALL_THISCALL);
	engine->RegisterObjectMethod("graph", "void shr_arc_set(string var_name, double def = 0)", asMETHOD(graph, shared_arcs_variable_set), asCALL_THISCALL);
	engine->RegisterObjectMethod("graph", "void shr_arc_del(string var_name)", asMETHOD(graph, shared_arcs_variable_del), asCALL_THISCALL);

	engine->RegisterObjectMethod("node", "void set_position(float x, float y)", asMETHODPR(graph::node, set_position, (float, float), void), asCALL_THISCALL);
	engine->RegisterObjectMethod("node", "void set_size(float size)", asMETHODPR(graph::node, set_size, (float), void), asCALL_THISCALL);
	engine->RegisterObjectMethod("node", "void adj_reset()", asMETHOD(graph::node, adj_reset), asCALL_THISCALL);
	engine->RegisterObjectMethod("node", "node@ adj_next()", asMETHOD(graph::node, adj_next), asCALL_THISCALL);
	engine->RegisterObjectMethod("node", "arc@ adj_arc()", asMETHOD(graph::node, adj_arc), asCALL_THISCALL);

	engine->RegisterObjectMethod("node", "string id()", asMETHOD(graph::node, id), asCALL_THISCALL);
	engine->RegisterObjectMethod("node", "bool var_chk(string name)", asMETHOD(graph::node, chk), asCALL_THISCALL);
	engine->RegisterObjectMethod("node", "void var_set(string name, double value = 0)", asMETHOD(graph::node, set), asCALL_THISCALL);
	engine->RegisterObjectMethod("node", "double var_get(string name)", asMETHOD(graph::node, get), asCALL_THISCALL);
	engine->RegisterObjectMethod("node", "void var_del(string name)", asMETHOD(graph::node, del), asCALL_THISCALL);
	engine->RegisterObjectMethod("node", "bool shr_chk(string name)", asMETHOD(graph::node, shr_chk), asCALL_THISCALL);
	engine->RegisterObjectMethod("node", "void shr_set(string name, double value = 0)", asMETHOD(graph::node, shr_set), asCALL_THISCALL);
	engine->RegisterObjectMethod("node", "double shr_get(string name)", asMETHOD(graph::node, shr_get), asCALL_THISCALL);

	engine->RegisterObjectMethod("arc", "void swap()", asMETHOD(graph::arc, swap), asCALL_THISCALL);
	engine->RegisterObjectMethod("arc", "node@ get_from()", asMETHOD(graph::arc, get_from), asCALL_THISCALL);
	engine->RegisterObjectMethod("arc", "node@ get_to()", asMETHOD(graph::arc, get_to), asCALL_THISCALL);

	engine->RegisterObjectMethod("arc", "string id()", asMETHOD(graph::arc, id), asCALL_THISCALL);
	engine->RegisterObjectMethod("arc", "bool var_chk(string name)", asMETHOD(graph::arc, chk), asCALL_THISCALL);
	engine->RegisterObjectMethod("arc", "void var_set(string name, double value = 0)", asMETHOD(graph::arc, set), asCALL_THISCALL);
	engine->RegisterObjectMethod("arc", "double var_get(string name)", asMETHOD(graph::arc, get), asCALL_THISCALL);
	engine->RegisterObjectMethod("arc", "void var_del(string name)", asMETHOD(graph::arc, del), asCALL_THISCALL);
	engine->RegisterObjectMethod("arc", "bool shr_chk(string name)", asMETHOD(graph::arc, shr_chk), asCALL_THISCALL);
	engine->RegisterObjectMethod("arc", "void shr_set(string name, double value = 0)", asMETHOD(graph::arc, shr_set), asCALL_THISCALL);
	engine->RegisterObjectMethod("arc", "double shr_get(string name)", asMETHOD(graph::arc, shr_get), asCALL_THISCALL);

	engine->RegisterGlobalProperty("graph g", g);
	}

void script_console::MessageCallback(const asSMessageInfo *msg, void *param)
	{
	const char *type = "ERR ";
	if (msg->type == asMSGTYPE_WARNING)
		type = "WARN";
	else if (msg->type == asMSGTYPE_INFORMATION)
		type = "INFO";

	std::cout << msg->section << "(Row: " << msg->row << ", Column: " << msg->col << "): " << type << " : " << msg->message << std::endl;
	}

void script_console::LineCallback(asIScriptContext * ctx, std::chrono::system_clock::time_point *timeout_base)
	{
	// If the time out is reached we abort the script
	if (std::chrono::system_clock::now() > *timeout_base) { ctx->Abort(); }

	// It would also be possible to only suspend the script,
	// instead of aborting it. That would allow the application
	// to resume the execution where it left of at a later 
	// time, by simply calling Execute() again.
	}

void script_console::configure_engine()
	{
	RegisterStdString(engine);
	//RegisterStdStringUtils(engine);
	RegisterScriptArray(engine, true);
	RegisterScriptFile(engine);
	RegisterScriptFileSystem(engine);
	RegisterScriptMath(engine);
	
	RegisterScriptPrint(engine);
	RegisterScriptGraph(engine);
	aatc::RegisterAllContainers(engine);
	}

bool script_console::compile(std::string & script)
	{
	int r;
	// Add the script sections that will be compiled into executable code.
	// If we want to combine more than one file into the same script, then 
	// we can call AddScriptSection() several times for the same module and
	// the script engine will treat them all as if they were one. The script
	// section name, will allow us to localize any errors in the script code.
	asIScriptModule *mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
	r = mod->AddScriptSection("script", &script[0], script.size());
	if (r < 0)
		{
		std::cout << "AddScriptSection() failed" << std::endl;
		return false;
		}

	// Compile the script. If there are any compiler messages they will
	// be written to the message stream that we set right after creating the 
	// script engine. If there are no errors, and no warnings, nothing will
	// be written to the stream.
	r = mod->Build();
	if (r < 0)
		{
		std::cout << "Build() failed" << std::endl;
		return false;
		}

	// The engine doesn't keep a copy of the script sections after Build() has
	// returned. So if the script needs to be recompiled, then all the script
	// sections must be added again.

	// If we want to have several scripts executing at different times but 
	// that have no direct relation with each other, then we can compile them
	// into separate script modules. Each module use their own namespace and 
	// scope, so function names, and global variables will not conflict with
	// each other.

	return true;
	}

bool script_console::run(std::string &script)
	{
	int r;
	if (!compile(script)) { return false; }


	// Find the function for the function we want to execute.
	asIScriptFunction *func = engine->GetModule(0)->GetFunctionByDecl("void script()");
	if (!func)
		{
		std::cout << "The function \"void script()\" was not found." << std::endl;
		return false;
		}

	// Prepare the script context with the function we wish to execute. Prepare()
	// must be called on the context before each new script function that will be
	// executed. Note, that if you intend to execute the same function several 
	// times, it might be a good idea to store the function returned by 
	// GetFunctionByDecl(), so that this relatively slow call can be skipped.
	r = ctx->Prepare(func);
	if (r < 0)
		{
		std::cout << "Failed to prepare the context." << std::endl;
		return false;
		}

	std::cout << "Executing the script." << std::endl;
	std::cout << "---" << std::endl;
	timeout_base = std::chrono::system_clock::now() + timeout_duration;
	r = ctx->Execute();
	std::cout << "---" << std::endl;

	if (r != asEXECUTION_FINISHED)
		{
		// The execution didn't finish as we had planned. Determine why.
		if (r == asEXECUTION_ABORTED)
			std::cout << "The script was aborted before it could finish. Probably it timed out." << std::endl;
		else if (r == asEXECUTION_EXCEPTION)
			{
			std::cout << "The script ended with an exception." << std::endl;

			// Write some information about the script exception
			asIScriptFunction *func = ctx->GetExceptionFunction();
			std::cout << "func: " << func->GetDeclaration() << std::endl;
			std::cout << "modl: " << func->GetModuleName() << std::endl;
			std::cout << "sect: " << func->GetScriptSectionName() << std::endl;
			std::cout << "line: " << ctx->GetExceptionLineNumber() << std::endl;
			std::cout << "desc: " << ctx->GetExceptionString() << std::endl;
			}
		else
			std::cout << "The script ended for some unforeseen reason (" << r << ")." << std::endl;
		}
	else
		{
		// Retrieve the return value from the context
		float returnValue = ctx->GetReturnFloat();
		std::cout << "The script function returned: " << returnValue << std::endl;
		}
	}

void script_console::help()
	{
	;
	}

script_console::script_console(bool & success, graph* g) : g(g)
	{
	int r;
	timeout_duration = std::chrono::milliseconds(10000);

	success = true;
	engine = asCreateScriptEngine();
	if (!engine) { success = false; return; }

	// The script compiler will write any compiler messages to the callback.
	engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

	// Configure the script engine with all the functions, 
	// and variables that the script should be able to use.
	configure_engine();

	// Create a context that will execute the script.
	ctx = engine->CreateContext();
	if (!ctx)
		{
		std::cout << "Failed to create the context." << std::endl;
		engine->Release(); success = false; return;
		}

	// We don't want to allow the script to hang the application, e.g. with an
	// infinite loop, so we'll use the line callback function to set a timeout
	// that will abort the script after a certain time. Before executing the 
	// script the timeOut variable will be set to the time when the script must 
	// stop executing. 
	r = ctx->SetLineCallback(asFUNCTION(LineCallback), &timeout_base, asCALL_CDECL);
	if (r < 0)
		{
		std::cout << "Failed to set the line callback function." << std::endl;
		ctx->Release(); engine->Release(); success = false; return;
		}
	}

script_console::~script_console()
	{
	// We must release the contexts when no longer using them
	ctx->Release();

	// Shut down the engine
	engine->ShutDownAndRelease();
	}


bool script_console::open()
	{
	std::string script;
	while (true)
		{
		std::string row;
		std::getline(std::cin, row);
		if (row == "$$") { break; }
		script += row;
		}
	return run(script);
	}


bool script_console::load(std::string &fname)
	{
	std::ifstream file(fname);
	if (!file) { return false; }

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string script = buffer.str();

	return run(script);
	}
