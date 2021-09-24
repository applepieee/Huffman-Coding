#include<iostream>
#include<vector>
#include <limits.h>
#include<math.h>
#include<algorithm>
#include<string>
using namespace std; 
// Huffman Tree Node
struct HuffNode {
	int data=0;
	char symbol='0';
	string code="";
	bool min;//if true means it is min
	HuffNode *leftChild;
	HuffNode *rightChild;
};
// min-heap class
template <class T>
class MinHeap {
	public:
		MinHeap():heapSize(1){}//initialize
		void Push(T&);
		void HeapDecreaseKey(int,int);
		void print_tree();
		int complete_tree_max_level();
		friend int FindMin(); 
		T* BuildHuffmanTree();
		int getHeapSize();
		int getMaxLevel(T*);
		void decoded(string,int);
		void coding(T*,string);
	private:
		vector<HuffNode> A;
		int heapSize; // number of elements in heap
};
template <class T>
int MinHeap<T>::getHeapSize()
{
	return heapSize-1;
}

template <class T>
int MinHeap<T>::getMaxLevel(T* node)//cal max height(root height)
{
	if(node==NULL) return -1;
	else
	{
		int l_height=getMaxLevel(node->leftChild);
		int r_height=getMaxLevel(node->rightChild);
		return max(l_height,r_height)+1;
	}
	
}

template <class T>
void MinHeap<T>::coding(T* node,string str)//encode
{
	if(node)
	{
		if(node->symbol != '0')
		{
			node->code=str;
			cout << node->symbol << " : " << str << endl;
		}
		coding(node->leftChild,str+"1");
		coding(node->rightChild,str+"0");
	}
}
template <class T>
void MinHeap<T>::Push(T& node)//insert
{
	int key=node.data;
	A.reserve(++heapSize);//init heapsize=1
	A.resize(heapSize);
	A[heapSize-1].data=INT_MAX;
	A[heapSize-1].symbol=node.symbol;
	HeapDecreaseKey(heapSize-1,key);
}

template<class T>
void MinHeap<T>::HeapDecreaseKey(int i,int key)//insert
{
	if(key > A[i].data)
		cout << "new key is bigger than the current key" << endl;
	else
	{
		A[i].data=key;
		while(i>1 && A[i/2].data > A[i].data)//compare with parent
		{
			HuffNode temp=A[i];
			A[i]=A[i/2];
			A[i/2]=temp;
			i/=2;
		}
	}	
}

template<class T>
void MinHeap<T>::print_tree()//print heap
{
	int endNumber=1;
	for(int i=1; i<heapSize; i++)
	{
		cout << A[i].data << "__";	
		if(i==endNumber)
		{
			cout << endl;	
			endNumber=endNumber*2+1;	
		}
	}
}

template<class T>
int MinHeap<T>::complete_tree_max_level()//max level
{
	return floor(log(heapSize-1)/log(2));
}

int FindMin(vector<HuffNode>& A)//finf min index
{
	HuffNode minNode;
	int index;
	for(int i=1; i<A.size(); i++)
		if(A[i].min != true)
		{
			minNode=A[i];
			index=i;
		}
	
	for(int i=1; i<A.size(); i++)
		if(A[i].min != true && A[i].data <  minNode.data)
		{
			minNode=A[i];
			index=i;
		}
	A[index].min=true;		
	return index;		
}

template<class T>
T* MinHeap<T>::BuildHuffmanTree()//find two min number and add them and put result into vector A
{
	HuffNode nodeParent;
	int min1_index,min2_index;
	int times=heapSize-2;
	A.reserve(2*A.size()-2);//should allocate here or the memeory space would reallocate leads memory address error
	while(times > 0)
	{
		min1_index=FindMin(A);			
		min2_index=FindMin(A);	
	
		nodeParent.data=A[min1_index].data+A[min2_index].data;
	
		nodeParent.leftChild=&A[min2_index];
		nodeParent.rightChild=&A[min1_index];
		
		A.resize(A.size()+1);
		A[A.size()-1]=nodeParent;
	
		times--;
	}
	heapSize=A.size();
	return &A[A.size()-1];
}

template<class T>
void print_preorder(T* node)
{
	if(node)
	{
		cout << node->data << " ";
		print_preorder(node->leftChild);
		print_preorder(node->rightChild);
	}
}

template<class T>
void print_inorder(T* node)
{
	if(node !=nullptr)
	{
		print_inorder(node->leftChild);
		cout << node->data << " ";
		print_inorder(node->rightChild);
	}
}
template<class T>
void MinHeap<T>::decoded(string str,int size)
{
	string s="";
	for(int i=0; i<str.length(); i++)
	{
		s+=str[i];
		for(int j=1; j<=size; j++)
			if(s==A[j].code)
			{
				cout << A[j].symbol;
				s="";
				break;
			}
	}
}
int main()
{
	//int size;//no.1 question
	//cout << "enter size" << endl;//no.1 question
	//cin >> size;//no.1 question
	string input;
	cout << "Input a string: ";
	cin >> input;

	vector<HuffNode> huffArray;
	HuffNode leaf_node;
	leaf_node.data=1;
	leaf_node.symbol=input[0];
	huffArray.push_back(leaf_node);

	for(int i=1; i<input.length(); i++)//cal freq and symbol
	{
		bool find=false;
		int j;
		for(j=0; j<huffArray.size(); j++)
			if(huffArray[j].symbol == input[i])
			{
				find=true;
				break;
			}	
		if(find)
			huffArray[j].data+=1;
		else
		{
			HuffNode leaf_node;// C++ style structure declaration
			leaf_node.data=1;
			leaf_node.symbol=input[i];
			huffArray.push_back(leaf_node);
		}	
	}
	
	for(int i=1; i<huffArray.size(); i++)//alphabetical order
	{
		int j=i-1;
		HuffNode temp=huffArray[i];
		while(j>=0 && huffArray[j].symbol > huffArray[i].symbol)
		{
			huffArray[i]=huffArray[j];
			j--;	
		}
		huffArray[j+1]=temp;
	}
	cout << "Your input size[int]: " << huffArray.size() << endl;
	cout << "Your input symbol[char]: " ;
	for(int i=0; i<huffArray.size(); i++)
		cout << huffArray[i].symbol << " ";
	cout << endl << "Your input frequency[int]: " ;
	for(int i=0; i<huffArray.size(); i++)
		cout << huffArray[i].data << " ";
	cout << endl;
	cout <<"======================="<<endl;
	//no.1 question		
	//char *arr = new char[size];
	//int *freq = new int [size];
	// Read
	/*
	cout << "enter symbol" << endl;
	for (int i = 0; i < size; i++) cin >> arr[i];
	cout << "enter freq" << endl;
	for (int i = 0; i < size; i++) cin >> freq[i];
	*/
	MinHeap<HuffNode> min_heap;
	// Initalize the leaf node
	for (int i = 0; i < huffArray.size(); i++) 
	{
		/*no.1 question
		HuffNode leaf_node;// C++ style structure declaration
		leaf_node.data = freq[i];
		leaf_node.symbol = arr[i];
		*/
		min_heap.Push(huffArray[i]);
	}
	cout << "MinHeap Tree:" << endl;
	min_heap.print_tree();
	cout << endl << "Max Level: " << min_heap.complete_tree_max_level() << endl;
	cout <<"======================="<<endl;
	HuffNode* root=min_heap.BuildHuffmanTree();
	cout <<"Huffman Tree: " << endl <<"PreOrder: ";
	print_preorder(root);
	cout << endl <<"InOrder: ";
	print_inorder(root);
	cout << endl <<"Max Level: ";
	cout << min_heap.getMaxLevel(root) << endl;
	cout << "Number of Node: ";
	cout << min_heap.getHeapSize() << endl;
	cout <<"======================="<<endl;
	cout << "Huffman Coding: "<< endl;//encode
	min_heap.coding(root,"");//encode
	cout <<"======================="<<endl;
	string str;//decode
	cout << endl;
	cout << "Input sequence for decode: ";//decode
	cin >> str;//decode
	cout << "Decoded Huffman Data: "<< endl;//decode
	min_heap.decoded(str,huffArray.size());//decode
	return 0;
}
