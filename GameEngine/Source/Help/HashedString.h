#pragma once

class HashedString
{
public:
	HashedString(char const * const pString);

	unsigned int GetId() const;

	const string & GetString() const;

	static void* hash(char const * pString);

	bool operator< (HashedString const & str) const;
	bool operator== (HashedString const & str) const;

private:

	void* m_id;
	string m_string;
};

__forceinline HashedString::HashedString(char const * const pString)
	: m_id{ hash(pString) }, m_string{ pString }
{
}

__forceinline unsigned int HashedString::GetId() const
{
	return reinterpret_cast<unsigned int>(m_id);
}

__forceinline const string & HashedString::GetString() const
{
	return m_string;
}

__forceinline bool HashedString::operator< (HashedString const & str) const
{
	return (GetId() < str.GetId());
}

__forceinline bool HashedString::operator== (HashedString const & str) const
{
	return (GetId() == str.GetId());
}