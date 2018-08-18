//
//  ape_emotion.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018 alan king. All rights reserved.
//

#include <atomic>

#include "NeurEmotion.hpp"

namespace nsAI {

	class CTagGenerator : public CNoCopyable
	{
	public:
		CTagGenerator() : CTagGenerator(0)
		{
		}
		CTagGenerator(size_t tagVal)
		{
			m_tagUnique.store(tagVal);
		}
		inline size_t getUniqueTag()
		{
			return m_tagUnique++;
		}
		~CTagGenerator() final = default;
	private:
		std::atomic_size_t m_tagUnique;
	};
}

nsAI::nsNeuronal::CEmotion::CEmotion(CEmotion_E t, size_t h) : CEmotion(static_cast<size_t>(t), h)
{

}

nsAI::nsNeuronal::CEmotion::CEmotion(size_t t, size_t h) : m_tag(t), m_hint(h)
{
    
}

std::string nsAI::nsNeuronal::CEmotion::echo(size_t tagval)
{
	std::string s = "tag unkown";
	if (tagval < EMOTION_E_MAX)
	{
		switch (static_cast<CEmotion_E>(tagval))
		{
		case CEmotion_E::input_absence:
			s = "input absense";
			break;
		case CEmotion_E::input_txt:
			s = "input text";
			break;
		case CEmotion_E::instinct_sleep:
			s = "instinct sleep";
			break;
		case CEmotion_E::instinct_crying:
			s = "instinct crying";
			break;
		case CEmotion_E::instinct_idle:
			s = "instinct idle";
			break;
		default:
			break;
		}
	}

	return s;
}

size_t nsAI::nsNeuronal::CEmotion::getUniqueTag()
{
	static CTagGenerator gen(EMOTION_E_MAX);
	return gen.getUniqueTag();
}

std::string nsAI::nsNeuronal::CEmotionText::echo() const {
	return CEmotion::echo(static_cast<size_t>(CEmotion_E::input_txt)) + " char : " + m_valChar;
}
