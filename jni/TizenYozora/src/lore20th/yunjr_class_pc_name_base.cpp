
#include "yunjr_class_pc_name_base.h"
#include "yunjr_res_string.h"

#include "util/sm_util_sena.h"

////////////////////////////////////////////////////////////////////////////////
// serializing method

bool yunjr::PcNameBase::_load(const smutil::ReadStream& stream)
{
	if (stream.read((void*)_name, sizeof(_name)) == sizeof(_name))
	{
		// �ٸ� ��� ������ ���� �����ϱ� ����
		setName(_name);
		return true;
	}
	else
	{
		return false;
	}
}

bool yunjr::PcNameBase::_save(const smutil::WriteStream& stream) const
{
	return (stream.write((void*)_name, sizeof(_name)) == sizeof(_name));
}

////////////////////////////////////////////////////////////////////////////////
// public method

yunjr::PcNameBase::PcNameBase(void)
{
	_name[0] = 0;
}

yunjr::PcNameBase::~PcNameBase(void)
{
}

bool yunjr::PcNameBase::isValid(void) const
{
	return (_name[0] != 0);
}

void yunjr::PcNameBase::setName(const char* sz_name)
{
	sena::strncpy(_name, sz_name, sizeof(_name)-1);

	bool has_jongsung = resource::hasJongsung(resource::getLastMultiByte(_name));

	m_name_subject1  = _name;
	m_name_subject1 += (has_jongsung) ? "��" : "��";

	m_name_subject2  = _name;
	m_name_subject2 += (has_jongsung) ? "��" : "��";

	m_name_object    = _name;
	m_name_object   += (has_jongsung) ? "��" : "��";
}

const char* yunjr::PcNameBase::getName(JOSA method) const
{
	static const char* sz_none = "";

	switch (method)
	{
	case JOSA_NONE:
		return _name;
	case JOSA_SUB:
		return m_name_subject1;
	case JOSA_SUB2:
		return m_name_subject2;
	case JOSA_OBJ:
		return m_name_object;
	default:
		assert(false);
		return sz_none;
	}
}