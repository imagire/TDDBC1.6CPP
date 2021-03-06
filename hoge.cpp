#include <gtest/gtest.h>

typedef struct {
	int key;
	int value;
} KeyValue;

class CKeyValue{
private:
	int m_count;
	KeyValue* values;


public:
	CKeyValue():m_count(0){
		values = NULL;
	}

	~CKeyValue() {
		if(values != NULL)
			delete[] (values);
	}

	KeyValue *find(int key){
		for(int i = 0; i < m_count; i++){
			if(values[i].key == key) return &values[i];
		}
		return NULL;
	}

	void put(int key, int value) {
		// すでにkeyが存在していたら、追加しないで、valueを上書き
		KeyValue *p = find(key);
		if(NULL != p){
				p->value = value;
				return;
		}

		KeyValue* tmp;
		tmp = new KeyValue[m_count+1];
		memcpy(tmp, values, sizeof(KeyValue)*m_count);
		delete[] (values);
		values = tmp;

		values[m_count].key = key;
		values[m_count].value = value;
		m_count++;
	}

	void puts(const KeyValue* v, int n) {
		for(int i=0; i < n; i++){
			put((v+i)->key, (v+i)->value);
		}
	}


	int get(int key) {
		KeyValue *p = find(key);

		return (p == NULL) ? -1 : p->value;
	}

	int remove(int key){
		for(int i = 0; i < m_count; i++){
			if(values[i].key == key){
				values[i].key = values[m_count-1].key;
				values[i].value = values[m_count-1].value;
				m_count--;
				return 0;
			}
		}

		return -1;
	}

	int dump(const KeyValue* &v) {
		v = reinterpret_cast<const KeyValue*>(&values[0]);
		return count();
	}

	int count() {
		return m_count;
	}
};



TEST(GetValue, ValueNotSet)
{
	CKeyValue kv;
    EXPECT_EQ(-1, kv.get(2));
}


TEST(GetValue, key2)
{
	CKeyValue kv;
	kv.put(2,2);
    EXPECT_EQ(2, kv.get(2));
}


TEST(GetValue, key3)
{
	CKeyValue kv;
	kv.put(2,3);
	kv.put(3,4);
    EXPECT_EQ(3, kv.get(2));
    EXPECT_EQ(4, kv.get(3));
}

TEST(put, Override)
{
	CKeyValue kv;

	kv.put(1,2);
	kv.put(1,1);

	EXPECT_EQ(1, kv.get(1));
}

TEST(put, CountInOverride)
{
	CKeyValue kv;

	kv.put(1,2);
	kv.put(1,1);

	EXPECT_EQ(1, kv.count());
}

TEST(put, OverrideAfterOtherKeys)
{
	CKeyValue kv;

	kv.put(1,2);
	kv.put(2,3);
	kv.put(4,5);
	kv.put(6,7);
	kv.put(1,8);

	EXPECT_EQ(8, kv.get(1));
	EXPECT_EQ(3, kv.get(2));
	EXPECT_EQ(5, kv.get(4));
	EXPECT_EQ(7, kv.get(6));
}

TEST(puts, Simple)
{
	CKeyValue kv;

	KeyValue v[]={
		{1,2},
		{3,4}
	};

	kv.puts(v, 2);

	EXPECT_EQ(2, kv.count());

	EXPECT_EQ(2, kv.get(1));
	EXPECT_EQ(4, kv.get(3));
}

TEST(puts, DuplicatedKey)
{
	CKeyValue kv;

	KeyValue v[]={
		{1,2},
		{1,4}
	};

	kv.puts(v, 2);

	EXPECT_EQ(1, kv.count());

	EXPECT_EQ(4, kv.get(1));
}

TEST(puts, OverWrite)
{
	CKeyValue kv;

	KeyValue v[]={
		{1,2},
		{3,4},
		{1,4}
	};

	kv.put(1, 1);
	kv.puts(v, 3);

	EXPECT_EQ(2, kv.count());

	EXPECT_EQ(4, kv.get(1));
}

TEST(puts, SetNull)
{
	CKeyValue kv;

	kv.puts(NULL, 0);

	EXPECT_EQ(0, kv.count());
}

TEST(find, NotPut)
{
	CKeyValue kv;

	EXPECT_EQ(NULL, kv.find(0));
}

TEST(find, Simple)
{
	CKeyValue kv;

	kv.put(1, 2);

	KeyValue *p = kv.find(1);

	EXPECT_EQ(1, p->key);
	EXPECT_EQ(2, p->value);
}

TEST(Dump, DumpCount)
{
	CKeyValue kv;

	int n;
	const KeyValue *v;
	
	n = kv.dump(v);
	EXPECT_EQ(0, n);

	kv.put(2,3);
	kv.put(3,4);

	n = kv.dump(v);
	EXPECT_EQ(2, n);
}

TEST(Dump, DumpValue)
{
	CKeyValue kv;
	kv.put(2,3);
	kv.put(3,4);

	int n;
	const KeyValue *v;
		
	n = kv.dump(v);

	int n2 = 0;
	int n3 = 0;

	for(int i = 0; i < n; i++){
		if(v[i].key == 2){    EXPECT_EQ(3, v[i].value);n2++;}
		if(v[i].key == 3){    EXPECT_EQ(4, v[i].value);n3++;}
	}

	EXPECT_EQ(1, n2);
	EXPECT_EQ(1, n3);
}


TEST(Delete, SimpleDelete)
{
	CKeyValue kv;

	kv.put(1,2);
	int ret = kv.remove(1);
	EXPECT_EQ(0, ret);
}


TEST(Delete, NotExist)
{
	CKeyValue kv;

	kv.put(1,2);

	int ret0 = kv.remove(0);
	ASSERT_NE(0, ret0);
}


TEST(GetCount, NotSet) {
	int ret;

	CKeyValue kv;

	ret = kv.count();
	EXPECT_EQ(0, ret);
}


TEST(GetCount, SimpleReturn) {
	int ret;

	CKeyValue kv;

	kv.put(1,2);

	ret = kv.count();
	EXPECT_EQ(1, ret);
}


TEST(Delete, DeleteDeletedKey)
{
	CKeyValue kv;

	int ret0;

	kv.put(1,2);

	ret0 = kv.remove(1);
	EXPECT_EQ(0, ret0);

	ret0 = kv.remove(1);
	ASSERT_NE(0, ret0);
}


TEST(Delete, MultipleItem)
{
	CKeyValue kv;

	kv.put(1,2);
	kv.put(2,4);

	int ret;
		
	ret = kv.remove(1);
	EXPECT_EQ(0, ret);

	ret = kv.remove(2);
	EXPECT_EQ(0, ret);

	EXPECT_EQ(0, kv.count());
}




int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

	int ret = RUN_ALL_TESTS();

	return ret;
}
