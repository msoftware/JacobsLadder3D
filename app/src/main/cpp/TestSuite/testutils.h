
#ifndef TESTUTILS_H_
#define TESTUTILS_H_

#define ASSERT_IS_NULL(x,ret) if(TestUtils::isNotNull(x))return ret;
#define ASSERT_IS_NOT_NULL(x,ret) if(TestUtils::isNull(x))return ret;
#define ASSERT_ARE_NOT_NULL(x,y,z,p,ret) if(TestUtils::isNull(x) && TestUtils::isNull(y) && TestUtils::isNull(z) && TestUtils::isNull(p))return ret;

#define ASSERT_TRUE(x,ret) if(!x)return ret;
#define ASSERT_FALSE(x,ret) if(x)return ret;

#define ASSERT_GREATER_THAN(x,y,ret) if(!TestUtils::greaterThan(x,y))return ret;
#define ASSERT_LESS_THAN(x,y,ret) if(!TestUtils::lessThan(x,y))return ret;
#define ASSERT_IS_EQUAL(x,y,ret) if(!TestUtils::areNumbersEqual(x,y))return ret;
#define ASSERT_IS_NOT_EQUAL(x,y,ret) if(TestUtils::areNumbersEqual(x,y))return ret;

class TestUtils {

public:
	static bool areNumbersEqual(int first, int second){
		return first == second;
	}
	static bool greaterThan(int first, int second){
		return first > second;
	}
	static bool lessThan(int first, int second){
		return first < second;
	}
	static bool isNull(void *obj){
		return obj == 0;
	}
	static bool isNotNull(void *obj){
		return obj != 0;
	}

};



#endif /* TESTUTILS_H_ */
