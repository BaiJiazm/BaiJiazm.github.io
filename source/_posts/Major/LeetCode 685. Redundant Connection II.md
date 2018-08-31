---
title: 图、树、并查集、DFS综合题目
date: 2018/8/31 14:58:28 
tags: 
- DS
- Algorithm
---

### LeetCode 685. Redundant Connection II
好久没做Leetcode了，今天突发奇想，看到了一道综合性比较强的题目，花了一点时间自己做出来了，厉害！基本Leetcode题解我都是不会写的，直接在代码里有注释或方法及链接。具体可看[题解](https://github.com/BaiJiazm/DS-Algorithm)

### Leetcode 164. Maximum Gap

* [题目链接](https://leetcode.com/problems/redundant-connection-ii/description/)

这道题目很好的考查了树的性质和图的基本知识，以及运用这些性质结合DFS和并查集解决问题的能力。

一棵树可以看做有向图，n个节点有n-1条边，增加1条边后只有两种情况：
* 有一个节点有两个入度，其中一个必多余
    * 如果没有形成环，那么去掉最后一个符合题意
    * 如果此时有环，那么去掉在环中的那一条边即可。从该节点出发DFS即可
* 所有节点只有一个入度，那么增加的边肯定经过原根节点，必会有环，去掉任何一条边即可。利用并查集找到最后出现的那条边。

```C++
class Solution {
	public:
		vector<vector<int>>graph;
		int start;
		vector<int>ans;

		void buildGraph(vector<vector<int>>& edges) {
			graph.clear();
			graph.resize(edges.size()+1);
			for(int i=0; i<edges.size(); ++i) {
				graph[edges[i][0]].push_back(edges[i][1]);
			}
		}

		int dfs(int u) {
			for(auto &a: graph[u]) {
				if(a==start) {
					ans.push_back(u);
					ans.push_back(a);
					return 1;
				}
				if(dfs(a))
					return 1;
			}
			return 0;
		}

		vector<int> findRedundantDirectedConnection(vector<vector<int>>& edges) {
			int n=edges.size();
			vector<int> inDegree(n+1, 0);
			for(int i=0; i<n; ++i) {
				if(++inDegree[edges[i][1]]>1) {
					buildGraph(edges);
					start=edges[i][1];
					if(dfs(start))
						return ans;
					return edges[i];
				}
			}

			init(n);
			for(int i=0; i<n; ++i) {
				int p=edges[i][0];
				int c=edges[i][1];
				int pFa=findFa(p);
				int cFa=findFa(c);
				if(pFa==cFa)
					return edges[i];
				fa[cFa]=pFa;
			}
			return vector<int>();
		}

		vector<int>fa;
		void init(int n) {
			fa.resize(n+1);
			for(int i=0; i<=n; ++i) {
				fa[i]=i;
			}
		}
		int findFa(int c) {
			if(fa[c]==c)
				return c;
			return fa[c]=findFa(fa[c]);
		}
};
```