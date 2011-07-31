#include <gtest/gtest.h>

typedef struct {
	int key;
	int value;
} KeyValue;

class CKeyValue{
private:
	int count;
	KeyValue* values;
public:
	CKeyValue():count(0){
		values = NULL;
	}

	~CKeyValue() {
		if(values != NULL)
			delete[] (values);
	}

	void put(int key, int value) {
		KeyValue* tmp;
		tmp = new KeyValue[count+1];
		memcpy(tmp, values, sizeof(KeyValue)*count);
		delete[] (values);
		values = tmp;

		values[count].key = key;
		values[count].value = value;
		count++;
	}

	int get(int key) {
		for(int i = 0; i < count; i++){
			if(values[i].key == key) return values[i].value;
		}

		return -1;
	}

	int remove(int key){
		for(int i = 0; i < count; i++){
			if(values[i].key == key){
				values[i].key = values[count-1].key;
				values[i].value = values[count-1].value;
				count--;
				return 0;
			}
		}

		return -1;
	}

	const KeyValue* dump(int &n) {
		n = count;
		return reinterpret_cast<const KeyValue*>(&values[0]);
	}
};



TEST(GetValue, key2)
{
	CKeyValue kv;
	kv.put(2,2);
    ASSERT_EQ(2, kv.get(2));
}


TEST(GetValue, key3)
{
	CKeyValue kv;
	kv.put(2,3);
	kv.put(3,4);
    ASSERT_EQ(3, kv.get(2));
    ASSERT_EQ(4, kv.get(3));
}

TEST(Dump, keys)
{
	CKeyValue kv;
	kv.put(2,3);
	kv.put(3,4);

	int n;
	const KeyValue *ret = kv.dump(n);

	ASSERT_EQ(2, n);

	int n2 = 0;
	int n3 = 0;
	for(int i = 0; i < n; i++){
		if(ret[i].key == 2){    ASSERT_EQ(3, ret[i].value);n2++;}
		if(ret[i].key == 3){    ASSERT_EQ(4, ret[i].value);n3++;}
	}

	ASSERT_EQ(1, n2);
	ASSERT_EQ(1, n3);
}

TEST(Delete, delete1)
{
	CKeyValue kv;

	kv.put(1,2);

	int ret0 = kv.remove(0);
	ASSERT_NE(0, ret0);

	int ret = kv.remove(1);
	ASSERT_EQ(0, ret);
}

TEST(Delete, delete2)
{
	CKeyValue kv;

	kv.put(1,2);
	kv.put(2,4);

	int ret0 = kv.remove(1);
	ASSERT_EQ(0, ret0);
	ret0 = kv.remove(1);
	ASSERT_NE(0, ret0);

	int ret = kv.remove(2);
	ASSERT_EQ(0, ret);

	int n;
	const KeyValue *p = kv.dump(n);
	ASSERT_EQ(0, n);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

	int ret = RUN_ALL_TESTS();

	return ret;
}
