#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Data {
public:
	Data() {
		this->key[0] = 0;
		this->key[1] = 0;
		for (int i = 0; i < 4; ++i) {
			dataChar[i] = 'N';
		}
		for (int i = 0; i < 4; ++i) {
			dataInt[i] = 0;
		}
	}
	Data(int* key, char* arrChar, int* arrInt) {
		this->key[0] = key[0];
		this->key[1] = key[1];
		for (int i = 0; i < 4; ++i) {
			dataChar[i] = arrChar[i];
		}
		for (int i = 0; i < 4; ++i) {
			dataInt[i] = arrInt[i];
		}
	}
	int key[2];
	char dataChar[4];
	int dataInt[4];
};

istream& operator>>(istream& stream, Data& data) {
	for (int i = 0; i < 2; ++i) {
		stream >> data.key[i];
	}
	for (int i = 0; i < 4; ++i) {
		stream >> data.dataChar[i];
	}
	for (int i = 0; i < 4; ++i) {
		stream >> data.dataInt[i];
	}
	return stream;
}

ostream& operator<<(ostream& stream, Data& data) {
	cout << "Key: ";
	for (int i = 0; i < 2; ++i) {
		cout << data.key[i] << " ";
	}
	cout << "___ Data: ";
	for (int i = 0; i < 4; ++i) {
		cout << data.dataChar[i];
	}
	cout << " | ";
	for (int i = 0; i < 4; ++i) {
		cout << data.dataInt[i];
	}
	return stream;
}

bool operator<(Data lhs, Data rhs) {
	if (lhs.key[0] != rhs.key[0]) {
		return lhs.key[0] < rhs.key[0];
	}
	else {
		return lhs.key[1] < rhs.key[1];
	}
}

class List {
private:

	int size;

	class Node {
	public:
		Node() {
			this->pNext = nullptr;
			this->pBack = nullptr;
			data = Data();
		}
		Node(Data data) {
			this->pNext = nullptr;
			this->pBack = nullptr;
			this->data = data;
		}
		Data data;
		Node* pNext;
		Node* pBack;

	};

	Node* fast_passage_to_element(int index) {
		int middle = size / 2;
		Node* current = nullptr;
		if (index > middle) {
			current = tail;
			int counter = size - 1;
			while (current != nullptr) {
				if (counter == index) {
					return current;
				}
				current = current->pBack;
				counter--;
			}
		}
		else {
			current = head;
			int counter = 0;
			while (current != nullptr) {
				if (counter == index) {
					return current;
				}
				current = current->pNext;
				counter++;
			}
		}
	}

	//������� � ����������
	Node* merge(Node* first = nullptr, Node* second = nullptr) {
		if (first == nullptr) return second;
		if (second == nullptr) return first;

		Node* result;
		if (first->data < second->data) {
			result = first;
			result->pNext = merge(first->pNext, second);
		}
		else {
			result = second;
			result->pNext = merge(first, second->pNext);
		}
		return result;
	}

	//��������� �� �����
	Node* merge_sort(Node* node) {
		if (node == nullptr || node->pNext == nullptr) return node;
		Node* a = node;
		Node* b = node->pNext;
		while ((b != nullptr) && (b->pNext != nullptr)) {
			node = node->pNext;
			b = b->pNext->pNext;
		}
		b = node->pNext;
		node->pNext = nullptr;
		return merge(merge_sort(a), merge_sort(b));
	}

	Node* head;
	Node* tail;

public:
	List() {
		size = 0;
		head = nullptr;
		tail = nullptr;
	}

	int GetSize() {
		return size;
	}

	void FillingFromFile(string& path) {
		ifstream file;
		file.open(path);

		if (!file.is_open()) { //���� ���� �� ��������
			cout << endl << "���� ���������� �������!" << endl;
			return;
		}
		else { //���� ���� ��������
			Data temp;

			while (!file.eof()) {
				file >> temp;
				Insert(size, temp);
			}

			file.close();
		}
	}

	void Insert(int index, Data information) {
		if (head == nullptr) { //���� ������ ����
			Node* node = new Node;
			head = node;
			tail = node;
			this->head->data = information;
			size++;
		}
		else if (index == 0) { //���������� � ����� ������ ������
			Node* newNode = new Node;
			newNode->data = information;
			newNode->pNext = head;
			head->pBack = newNode;
			head = newNode;
			size++;
		}
		else if (index == size) { //���������� � ����� ������
			Node* newNode = new Node;
			newNode->data = information;
			tail->pNext = newNode;
			newNode->pBack = tail;
			tail = newNode;
			size++;
		}
		else if (index > size) { //���������� � �������, ������� ������ ������� ������
			int difference = index - size;
			for (int i = 0; i < difference; ++i) {
				Node* newNode = new Node;
				newNode->data = Data();
				tail->pNext = newNode;
				newNode->pBack = tail;
				tail = newNode;
				size++;
			}
			Node* newNode = new Node;
			newNode->data = information;
			tail->pNext = newNode;
			newNode->pBack = tail;
			tail = newNode;
			size++;
		}
		else { //������� ���������� �������� ��������� � �������� (����� �������)
			Node* current = fast_passage_to_element(index);
			Node* backNode = current->pBack;
			Node* newNode = new Node;
			newNode->data = information;
			newNode->pBack = backNode;
			newNode->pNext = current;
			backNode->pNext = newNode;
			current->pBack = newNode;
			size++;
		}
	}

	void Delete(int index) {
		if (head == nullptr) { //���� ������ ����
			cout << endl << "���������� ������� �������. ������ ����!" << endl;
		}
		else if (size == 1) {
			delete head;
			head = nullptr;
		}
		else if (index == 0) { //�������� �� ������ ������ ������
			Node* deleteNode = head;
			head = head->pNext;
			head->pBack = nullptr;
			delete deleteNode;
			size--;
		}
		else if (index == size - 1) { //�������� �� ����� ������
			Node* deleteNode = tail;
			tail = tail->pBack;
			tail->pNext = nullptr;
			delete deleteNode;
			size--;
		}
		else if (index > size) { //�������� ��������������� ��������
			cout << endl << "���������� ������� �������������� �������!" << endl;
		}
		else { //������� �������� �������� ��������� � �������� (����� �������)
			Node* current = fast_passage_to_element(index);
			current->pBack->pNext = current->pNext;
			current->pNext->pBack = current->pBack;
			delete current;
			size--;
		}
	}

	void SetElement(int index, Data information) {
		if (index >= size) {
			cout << endl << "���������� �������� �������������� �������!" << endl;
		}
		else {
			Node* current = fast_passage_to_element(index);
			current->data = information;
		}
	}

	void Print() {
		if (head == nullptr) {
			cout << endl << "������ ����!" << endl;
		}
		else {
			Node* current = head;
			cout << endl;
			int index = 0;
			while (current != nullptr) {
				cout << "[" << index << "] " << current->data << endl;
				current = current->pNext;
				index++;
			}
			cout << endl;
		}
	}

	void Clear() {
		size = 0;
		delete head;
		head = nullptr;
	}

	void MergeSort() {
		this->head = merge_sort(head);
	}

};

void PrintMenu() {
	cout << endl << "�������� �������:" << endl;
	cout << "[1] - ����� ������" << endl;
	cout << "[2] - ����� ������� ������" << endl;
	cout << "[3] - ���������� ������ � ����������" << endl;
	cout << "[4] - ���������� ������ �� �����" << endl;
	cout << "[5] - �������� ������� � �������� �������" << endl;
	cout << "[6] - ������� ������� � �������� �������" << endl;
	cout << "[7] - �������� ������� � �������� �������" << endl;
	cout << "[8] - ���������� ��������" << endl;
	cout << "[9] - �������� ���� ������" << endl;
	cout << "���� �������: ";
}

int main() {
	setlocale(LC_ALL, "Russian");

	List list;
	Data temp;
	string path = "C:\\Users\\buddy\\source\\repos\\doubly linked list (merge sort)\\file.txt";

	int command = 0;
	PrintMenu();
	while (cin >> command) {
		if (command == 1) {
			list.Print();
		}
		else if (command == 2) {
			cout << endl << "������ ������: " << list.GetSize() << endl;
		}
		else if (command == 3) {
			cout << "������� ���������� ������, ������� ������ ��������: ";
			int count;
			cin >> count;
			cout << "������� ������:" << endl;
			for (int i = 0; i < count; ++i) {
				cin >> temp;
				list.Insert(list.GetSize(), temp);
			}
		}
		else if (command == 4) {
			list.FillingFromFile(path);
		}
		else if (command == 5) {
			cout << "������� ������:" << endl;
			cin >> temp;
			int id = 0;
			cout << "������� �������:" << endl;
			cin >> id;
			list.Insert(id, temp);
		}
		else if (command == 6) {
			int id = 0;
			cout << "������� �������:" << endl;
			cin >> id;
			list.Delete(id);
		}
		else if (command == 7) {
			cout << "������� ������:" << endl;
			cin >> temp;
			int id = 0;
			cout << "������� �������:" << endl;
			cin >> id;
			list.SetElement(id, temp);
		}
		else if (command == 8) {
			list.MergeSort();
		}
		else if (command == 9) {
			list.Clear();
		}
		else { cout << endl << "����������� �������!" << endl; }
		PrintMenu();
	}
	return 0;
}