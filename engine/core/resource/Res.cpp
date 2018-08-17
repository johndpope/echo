#include "Res.h"
#include "engine/core/log/Log.h"
#include "engine/core/io/IO.h"
#include "engine/core/util/PathUtil.h"
#include <thirdparty/pugixml/pugixml.hpp>
#include <thirdparty/pugixml/pugiconfig.hpp>

namespace Echo
{
	static map<String, Res*>::type	g_ress;
	static map<String, Res::ResFun>::type g_resFuncs;

	Res::Res()
		: m_refCount(1)
		, m_isLoaded(false)
	{

	}

	Res::Res(const ResourcePath& path)
		: m_refCount(1)
		, m_isLoaded(false)
	{
		m_path = path;

		auto it = g_ress.find(path.getPath());
		if (it == g_ress.end())
		{
			g_ress[path.getPath()] = this;
		}
		else
		{
			EchoLogError("create resource multi times");
		}
	}

	Res* Res::createByClassName(const String& className)
	{
		const ResFun* resFun = getResFunByClassName(className);
		if (resFun)
		{
			Res* res = resFun->m_cfun();
			if (res)
				return res;
		}

		EchoLogError("Res::create failed. Unknown class [%s]", className.c_str());
		return nullptr;
	}

	Res::~Res()
	{
		auto it = g_ress.find(m_path.getPath());
		if (it != g_ress.end())
		{
			g_ress.erase(it);
		}
		else
		{
			EchoLogError("can't delete resource for cache");
		}
	}

	// bind methods to script
	void Res::bindMethods()
	{
		CLASS_BIND_METHOD(Res, getPath, DEF_METHOD("getPath"));
		CLASS_BIND_METHOD(Res, setPath, DEF_METHOD("setPath"));

		CLASS_REGISTER_PROPERTY(Res, "Path", Variant::Type::String, "getPath", "setPath");
	}

	// resister res
	void Res::registerRes(const String& className, const String& ext, RES_CREATE_FUNC cfun, RES_LOAD_FUNC lfun)
	{
		ResFun fun;
		fun.m_class = className;
		fun.m_ext = ext;
		fun.m_cfun = cfun;
		fun.m_lfun = lfun;

		g_resFuncs[ext] = fun;
	}

	// get res
	Res* Res::get(const ResourcePath& path)
	{
		auto it = g_ress.find(path.getPath());
		if (it != g_ress.end())
		{
			return it->second;
		}

		// get load fun
		String ext = PathUtil::GetFileExt(path.getPath(), true);
		map<String, Res::ResFun>::type::iterator itfun = g_resFuncs.find(ext);
		if (itfun != g_resFuncs.end())
		{
			Res* res = itfun->second.m_lfun(path);
			if (!res)
			{
				EchoLogError("Res::get file [%s] failed.", path.getPath().c_str());
			}

			return res;
		}

		return nullptr;
	}

	// create by extension
	ResPtr Res::createByFileExtension(const String& extension)
	{
		String ext = extension;
		map<String, Res::ResFun>::type::iterator itfun = g_resFuncs.find(ext);
		if (itfun != g_resFuncs.end())
		{
			Res* res = itfun->second.m_cfun();
			if (res)
				return res;	
		}

		EchoLogError("Res::create failed. Unknown extension [%s]", extension.c_str());
		return nullptr;
	}

	// get res fun by class
	const Res::ResFun* Res::getResFunByClassName(const String& className)
	{
		for (auto& it : g_resFuncs)
		{
			if (it.second.m_class == className)
			{
				return &it.second;
			}
		}

		return nullptr;
	}

	// release
	void Res::subRefCount()
	{
		m_refCount--;
		if (m_refCount <= 0)
		{
			ECHO_DELETE_T(this, Res);
		}
	}

	// load
	Res* Res::load(const ResourcePath& path)
	{
		MemoryReader reader(path.getPath());
		if (reader.getSize())
		{
			pugi::xml_document doc;
			if (doc.load_buffer(reader.getData<char*>(), reader.getSize()))
			{
				pugi::xml_node root = doc.child("res");
				if (root)
				{
					Res* res = ECHO_DOWN_CAST<Res*>(instanceObject(&root));
					res->setPath(path.getPath());

					g_ress[path.getPath()] = res;

					return res;
				}
			}
		}

		return nullptr;
	}

	// save
	void Res::save()
	{
		String fullPath = IO::instance()->getFullPath(m_path.getPath());

		pugi::xml_document doc;

		// declaration
		pugi::xml_node dec = doc.prepend_child(pugi::node_declaration);
		dec.append_attribute("version") = "1.0";
		dec.append_attribute("encoding") = "utf-8";

		// root node
		pugi::xml_node root = doc.append_child("res");

		root.append_attribute("name").set_value(getName().c_str());
		root.append_attribute("class").set_value(getClassName().c_str());
		savePropertyRecursive(&root, this, this->getClassName());

		doc.save_file(fullPath.c_str(), "\t", 1U, pugi::encoding_utf8);
	}
}