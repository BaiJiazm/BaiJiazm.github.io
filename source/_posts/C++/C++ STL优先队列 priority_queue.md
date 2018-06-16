---
title: C++ STL优先队列 priority_queue
date: 2018/3/7 11:25:52 
tags: 
- CPlusPlus
- STL
---

priority_queue模板声明带有三个参数:
`priority_queue<Type, Container, Functional>`
`Type` 为数据类型，
`Container` 为保存数据的容器，
`Functional` 为元素比较方式。
`Container` 必须是用数组实现的容器，比如 vector, deque 但不能用 list.
STL里面默认用的是 vector. 比较方式默认用 `operator<` , 后面俩个参数缺省的话，优先队列就是大顶堆，队头元素最大。

```C++
template<
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type>
> class priority_queue;
```

可参考[priority_queue的用法](https://blog.csdn.net/maverick1990/article/details/22413441)

##### 方法一

```C++
//********************************方法一
struct Node {
	int x,y;
	bool operator <(Node a) const  {	//必须加const
		return y < a.y;
	}
	bool operator >(Node a) const  {	//必须加const
		return y > a.y;
	}
};
//	priority_queue<Node> A;   //默认	大根堆
priority_queue<Node, vector<Node>, less<Node>>A;	//大根堆
priority_queue<Node, vector<Node>, greater<Node> > B;    //小根堆
```

##### 方法二

```C++
//********************************方法二：
struct Node {
	int x;
	int y;
};

bool operator<(const Node &a, const Node &b) {
	return a.x<b.x;			 //大顶堆
}

bool operator>(const Node &a, const Node &b) {
	return a.x>b.x;			//小顶堆
}

priority_queue<Node,vector<Node>,less<Node> > A;   	//大根堆
priority_queue<Node, vector<Node>, greater<Node> > B;    //小根堆
```

##### 方法三（节点是指针时只能是此方法）

非指针：

```C++
//********************************方法三：
struct Node {
	int x;
	int y;
};

struct cmp {
	bool operator()(Node a,Node b) {
		return  a.x > b.x;	//小顶堆
	}
};

struct cmp1 {
	bool operator()(Node a,Node b) {
		return  a.x < b.x;	//大顶堆
	}
};

priority_queue<Node,vector<Node>,cmp1 > A;   	//大根堆
priority_queue<Node, vector<Node>, cmp > B;    //小根堆
```

指针：

```C++
struct Node {
	int x;
	int y;
};
struct cmp {
	bool operator () (const Node*n1, const Node *n2) {
		return n1->x<n2->x;		//大顶推
	}
};
struct cmp1 {
	bool operator () (const Node *n1, const Node *n2) {
		return n1->x>n2->x;		//小顶推
	}
};

priority_queue<Node*, vector<Node*>, cmp > A;	//大根堆
priority_queue<Node*, vector<Node*>, cmp1 > B;  //小根堆
```

##### 统一测试

```C++
ostream & operator <<(ostream &out,const struct Node& n) {
	out<<"n.x="<<n.x<<"    n.y="<<n.y<<endl;
	return out;
}

const vector<Node>tn= {{1,1},{2,2},{3,3},{4,4},{5,5}};

void test() {
	for (auto &a:tn) {
		A.push(a);
		B.push(a);
	}

	cout<<"A:"<<endl;
	while(!A.empty()) {
		cout<<A.top();
		A.pop();
	}

	cout<<"B:"<<endl;
	while(!B.empty()) {
		cout<<B.top();
		B.pop();
	}
}

int main() {
	test();
	return 0;
}
```