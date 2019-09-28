#ifndef __C_MY_SHADER_H__
#define __C_MY_SHADER_H__

#include "defines.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class CMyShader
{
public:
	typedef unsigned int UINT;
	const static UINT SHADER_LOG_BUF_SIZE = 1024;


	CMyShader()
		:m_ShaderProgramID(0)
	{}

	CMyShader(const std::string& szVertexPath, const std::string& szFragmentPath)
	{
		InitWithFile(szVertexPath, szFragmentPath);
	}

	bool InitWithFile(const std::string& szVertexPath, const std::string& szFragmentPath)
	{
		if(szVertexPath.empty() || szFragmentPath.empty())
		{
			return false;
		}

		std::string szVertexCode = _LoadFile(szVertexPath);
		std::string szFragmentCode = _LoadFile(szFragmentPath);
		return InitWithCode(szVertexCode, szFragmentCode);
	}

	bool InitWithCode(const std::string& szVertexCode, const std::string& szFragmentCode)
	{
		if(szVertexCode.empty() || szFragmentCode.empty())
		{
			return false;
		}

		UINT nVertexID = glCreateShader(GL_VERTEX_SHADER);
		if (!_CompileShader(nVertexID, szVertexCode))
		{
			std::cout << _GetShaderError(nVertexID) << std::endl;
			return false;
		}

		UINT nFragmentID = glCreateShader(GL_FRAGMENT_SHADER);
		if (!_CompileShader(nFragmentID, szFragmentCode))
		{
			std::cout << _GetShaderError(nFragmentID) << std::endl;
			return false;
		}

		m_ShaderProgramID = glCreateProgram();
		if (!_LinkShader(m_ShaderProgramID, nVertexID, nFragmentID))
		{
			std::cout << _GetShaderError(m_ShaderProgramID) << std::endl;
			return false;
		}

		glDeleteShader(nVertexID);
		glDeleteShader(nFragmentID);

		return true;
	}

	void UseShader()
	{
		glUseProgram(m_ShaderProgramID);
	}

	[[nodiscard]] UINT GetShaderProgramID() const
	{
		return m_ShaderProgramID;
	}

	void SetBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_ShaderProgramID, name.c_str()), (int)value);
	}

	void SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_ShaderProgramID, name.c_str()), value);
	
	}
	void SetFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_ShaderProgramID, name.c_str()), value);
	}

	void SetFloat(const std::string& name, float v1, float v2) const
	{
		glUniform2f(glGetUniformLocation(m_ShaderProgramID, name.c_str()), v1, v2);
	}

	void SetFloat(const std::string& name, float v1, float v2, float v3) const
	{
		glUniform3f(glGetUniformLocation(m_ShaderProgramID, name.c_str()), v1, v2, v3);
	}

	void SetFloat(const std::string& name, float v1, float v2, float v3, float v4) const
	{
		glUniform4f(glGetUniformLocation(m_ShaderProgramID, name.c_str()), v1, v2, v3, v4);
	}

private:
	bool _CompileShader(UINT nShaderID, const std::string& szCode)
	{
		if (szCode.empty() || !nShaderID)
		{
			return false;
		}

		const char* szCStr = szCode.c_str();
		int iSucc = 0;
		glShaderSource(nShaderID, 1, &szCStr, nullptr);
		glCompileShader(nShaderID);
		glGetShaderiv(nShaderID, GL_COMPILE_STATUS, &iSucc);
		return bool(iSucc);
	}

	bool _LinkShader(UINT nProgramID, UINT nVertexID, UINT nFragmentID)
	{
		if (!nProgramID || !nVertexID || !nFragmentID)
		{
			return false;
		}

		glAttachShader(nProgramID, nVertexID);
		glAttachShader(nProgramID, nFragmentID);
		glLinkProgram(nProgramID);
		int iSucc = 0;
		glGetProgramiv(nProgramID, GL_LINK_STATUS, &iSucc);
		return bool(iSucc);
	}


	std::string _GetShaderError(UINT nShaderID)
	{
		char szInfoLog[SHADER_LOG_BUF_SIZE];
		glGetShaderInfoLog(nShaderID, SHADER_LOG_BUF_SIZE, nullptr, szInfoLog);
		return std::string(szInfoLog);
	}

	std::string _LoadFile(const std::string& szPath)
	{
		std::string szRet;
		if (szPath.empty())
		{
			return szRet;
		}

		std::ifstream oFileLoader;
		oFileLoader.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			oFileLoader.open(szPath);

			std::stringstream oStringSteam;
			oStringSteam << oFileLoader.rdbuf();

			oFileLoader.close();

			szRet = oStringSteam.str();
		}
		catch (std::ifstream::failure e)  // NOLINT(misc-throw-by-value-catch-by-reference)
		{
			std::cout << "_LoadFile Error :" << e.what() << std::endl;
		}
		return szRet;
	}

	UINT m_ShaderProgramID{};	//shader³ÌÐòid

};

#endif