
#ifndef  __ASSERT_H_20131002__
#define __ASSERT_H_20131002__

#include <assert.h>



#ifdef NDEBUG
	#define ASSERT(x)	(static_cast<void>(x))
#else
	#define ASSERT(x)	assert(x)
#endif //NDEBUG



#endif //__ASSERT_H_20131002__

