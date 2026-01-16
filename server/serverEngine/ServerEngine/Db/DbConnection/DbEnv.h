// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once


class DbEnv final
{
private:
	SQLHENV mHandle = nullptr;

public:
	DbEnv();
	~DbEnv();

public:
	SQLHENV GetHandle() const { return mHandle; }
};

inline std::shared_ptr<DbEnv> gDbEnv = nullptr;
