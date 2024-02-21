
#ifndef __PTRQUEUE_H_20131002__
#define __PTRQUEUE_H_20131002__




//遍历队列所有元素时的回调函数类型定义
//pData:	元素项
//pContext:	上下文
//返回false终止遍历
typedef bool (*PENUMPTRQUEUECALLBACK)(void* pData, void* pContext);

class CPtrQueue
{
public:
	CPtrQueue();
	~CPtrQueue();

public:
	//从队列头添加元素
	void PushHead(void* pData);

	//从队列尾添加元素
	void PushTail(void* pData);

	//从队列头得到元素并删除队列头节点
	void* PopHead();

	//从队列尾得到元素并删除队列尾节点
	void* PopTail();

	//得到队列头节点元素
	void* GetHead() const;
	
	//得到队列尾节点元素
	void* GetTail() const;

	//得到队列总节点个数
	int GetCount() const;

	//为队列设置当内存不足需要再分配时的增长步长		
	//nGrowBy：当内存不够用时一次性开辟多少内存空间
	//取值增大会提高效率，但过大会浪费空间
	void SetGrowBy(int nGrowBy);

	//删除队列中所用元素，但不释放内存空间
	void RemoveAll();

	//释放多余的内容，使用队列占用的内存空间与元素个数一致
	void FreeExtra();

	//判断当前队列是否为空
	bool IsEmpty() const;

	//从头节点逐项遍历到尾节点，每项结果由回调函数返回同时返回pContext
	void EnumpPtrQueue(PENUMPTRQUEUECALLBACK fpCallBack, void* pContext);

private:
	//得到当前节点下一个节点的内存位置
	void** GetNextPos(void** ppPos);
	
	//得到当前节点上一个节点的内存位置
	void** GetPrevPos(void** ppPos);	

private:
	void**	m_ppQueue;		//内部内存指针
	void**	m_ppHead;		//头节点指针
	void**	m_ppTail;		//尾节点指针
	int		m_nCount;		//队列节点总数
	int		m_nAlloc;		//已分配内存空间大小
	int		m_nGrowBy;		//空间不足时新开僻内存大小
};

#endif //#endif //__PTRQUEUE_H_20131002__
