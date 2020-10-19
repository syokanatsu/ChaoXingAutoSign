#pragma once
#include "framework.h"

class MiscThread : public nbase::FrameworkThread
{
public:
	MiscThread(enum ThreadId thread_id, const char* name)
		: FrameworkThread(name)
		, thread_id_(thread_id) {}

	~MiscThread(void) {}

private:
	/**
	* 虚函数，初始化线程
	* @return void	无返回值
	*/
	virtual void Init() override;

	/**
	* 虚函数，线程退出时，做一些清理工作
	* @return void	无返回值
	*/
	virtual void Cleanup() override;

private:
	enum ThreadId thread_id_;
};

class MainThread :
	public nbase::FrameworkThread
{
public:
	MainThread() :nbase::FrameworkThread("MainThread") {}
	virtual ~MainThread() {}
private:
	virtual void Init() override;
	virtual void Cleanup() override;

private:
	std::unique_ptr<MiscThread>	misc_thread_;	// 专门处理杂事的线程
};

