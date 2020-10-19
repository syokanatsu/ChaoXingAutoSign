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
	* �麯������ʼ���߳�
	* @return void	�޷���ֵ
	*/
	virtual void Init() override;

	/**
	* �麯�����߳��˳�ʱ����һЩ������
	* @return void	�޷���ֵ
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
	std::unique_ptr<MiscThread>	misc_thread_;	// ר�Ŵ������µ��߳�
};

