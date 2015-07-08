////////////////////////////////////////////////////////////////////////////////

#ifndef __IResManager_h_included__
#define __IResManager_h_included__


//!

//!
//!
enum EResPriority
{
	ERP_LOW,
	ERP_HIGH
};


//!

//!
//!
struct TResource
{
	enum EResState
	{
		INVALID,		//< ������ ������ ��� ������ (�����)
		QUEUED,			//< ������ ��������� � ������� �� ��������
		LOADING,		//< ������ � ������ ������ �����������
		FAILED,			//< �������� ������� �����������
		READY_TO_USE,	//< ������ ����� ������������ (�������� ������������)
		LOADED			//< ������ ��������� ��������
	};

	TResource() :
		pCreator	( NULL	),
		pResource	( NULL	),
		pFile		( NULL	),
		pBuffer		( NULL	),
		nSize		( 0		),
		nReadySize	( 0		),
		eState		( INVALID )
	{
	}

	CStr					sFileName;	/// ��� ������� (���� � �����)
	struct IResCreator *	pCreator;	//< ��������� �� ���������
	void *					pResource;
	void *					pFile;		//< ��������� �� ����
	byte *					pBuffer;	//< ���� � ������ (������)
	size_t					nSize;		//< ������ ����� �������
	size_t					nReadySize;	//< ������ ����������� ����� �������
	EResState				eState;		//< ��������� ��������
};


struct IResCreatorInfo
{
	const char * szInfo;
	const char * szExtension;
};


//!

//!
//!
struct IResCreator
{
	//! ������ ���������� � ����������
	virtual const IResCreatorInfo * GetInfo () const = 0;

	//! ������ ��������� �������, ����������� ��� ������ �� �������� �� ���������
	virtual void CreateResInstance( TResource ** ppOutResource ) = 0;

	//! ������ ������ ��� ���������� �����
	
	//! ����� ���� ��� ������ ��������� ������� ��� ���������� ����������������.
	//! � ���������� ���������� ����� ������ ����� ������ ������ �������� �� �����
	//! ������� ����� (�������� ������, �� ������� �� ����������� ����������)
	//! �� ���������� ������ � ��������� ������ ����� �������� pOutMaxSize ����
	//! ����� ���� ����� ������ ����� UpdateRes
	//! \param[in] pRes �������� �� ��������� �������.
	//! \param[in] nFileSize ������ �����
	//! \param[out] pOutMaxSize ������ ������������ ������� ( 0 <= pOutMaxSize <= nFileSize )
	//! \return ��������� �� ������, � ������� ���������� ��������� ����.
	//! \remarks ������ � ������ ����� �������������� �� ���������� ������, �� ����� � ���
	//! ��������� ��������� ����� �������� ����� ����� ������ UpdateRes
	//! \sa UpdateRes
	virtual size_t InitRes ( TResource * pRes, size_t nFileSize ) = 0;

	//! �������� ��������� ��������� ������

	//! ���������� �� 1 �� N ���, �� ����� �������� �������
	//! \param[in] pRes �������� �� ��������� �������.
	//! \param[in] nBytesWritten ���������� ���� ��������� � ������� ��������
	//! ������ UpdateRes
	//! \param[out] pOutMaxSize ������������ ���������� ���� ������� ����� ��������
	//! � ������������ ���������.
	//! \return ��������� �� ������, � ������� ���������� ����� ��������� ����.
	//! \remarks ���� ������� ������� NULL, �� ��� �������� ���������� ��������.
	//! ����� ���������� � ���� ������� - ����� ���� ������ ���������, ��� �����
	//! � ���� �������� �������� ������. � � ��� � ������ ������ �������� ��������
	//! ����� ��� ������ �� ������ ��������.
	//! \sa InitRes
	virtual size_t UpdateRes( TResource * pRes, size_t nBytesWritten ) = 0;

	virtual void FinalizeRes( TResource * pRes ) = 0;
};


//!

//!
//!
struct IResManager : public IBaseIntf
{
	//! ����������� ������ ���������� �������� (������� ������)
	virtual void RegCreator( IResCreator * pCreator )						= 0;

	//! �� ������� ���� �� �������� ������
	virtual bool Request( const char * szFile, TResource ** ppOutResource )		= 0;

	//! ������ ������ � ������� �� �������� � ����� �������
	virtual uint Demand( const char * szFile, TResource ** ppOutResource, EResPriority ePriority = ERP_LOW ) = 0;

	//! ������ ��������� �������
	virtual TResource::EResState GetResState( uint nQueueID ) const = 0;

	//! �������� �������
	//! ���� �������� ������� �����������, �� ����� ���������� �� ��� ������
	//! � ������� ��� ������ �� ����. ���� ������ (������) ���������, �� ������
	//! �������� ���������� �����������, �.�. �� ������ � �����������
	//! ����� ���� ���-�� ������ (� ������ ������ ������ ����� �� ������)
	virtual void RemoveRes( uint nQueueID ) = 0;

	//! ����� ����������. ����� �������� ������ ���� (��� �������� �������� � ������� ������)
	virtual void Update() = 0;
};

////////////////////////////////////////////////////////////////////////////////

#endif // #define __IResManager_h_included__

////////////////////////////////////////////////////////////////////////////////