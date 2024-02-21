// QR_Encode.h : CQR_Encode �N���X�錾����уC���^�[�t�F�C�X��`
// Date 2006/05/17	Ver. 1.22	Psytec Inc.

#ifndef AFX_QR_ENCODE_H__AC886DF7_C0AE_4C9F_AC7A_FCDA8CB1DD37__INCLUDED_
#define AFX_QR_ENCODE_H__AC886DF7_C0AE_4C9F_AC7A_FCDA8CB1DD37__INCLUDED_



// ���������x��
#define QR_LEVEL_L	0
#define QR_LEVEL_M	1
#define QR_LEVEL_Q	2
#define QR_LEVEL_H	3

// �f�[�^���[�h
#define QR_MODE_NUMERAL		0
#define QR_MODE_ALPHABET	1
#define QR_MODE_8BIT		2
#define QR_MODE_KANJI		3

// �o�[�W����(�^��)�O���[�v
#define QR_VRESION_S	0 // 1 �` 9
#define QR_VRESION_M	1 // 10 �` 26
#define QR_VRESION_L	2 // 27 �` 40

#define MAX_ALLCODEWORD	 3706 // 
#define MAX_DATACODEWORD 2956 // 
#define MAX_CODEBLOCK	  153 // 
#define MAX_MODULESIZE	  177 // 

// �r�b�g�}�b�v�`�掞�}�[�W��
#define QR_MARGIN	1
#define min(x,y) ((x)<(y)?(x):(y))

/////////////////////////////////////////////////////////////////////////////
typedef struct tagRS_BLOCKINFO
{
    int ncRSBlock;		// �q�r�u���b�N��
    int ncAllCodeWord;	// �u���b�N���R�[�h���[�h��
    int ncDataCodeWord;	// �f�[�^�R�[�h���[�h��(�R�[�h���[�h�� - �q�r�R�[�h���[�h��)

} RS_BLOCKINFO, *LPRS_BLOCKINFO;


/////////////////////////////////////////////////////////////////////////////
// QR�R�[�h�o�[�W����(�^��)�֘A���

typedef struct tagQR_VERSIONINFO
{
    int nVersionNo;	   // �o�[�W����(�^��)�ԍ�(1�`40)
    int ncAllCodeWord; // ���R�[�h���[�h��

    // �ȉ��z��Y���͌�������(0 = L, 1 = M, 2 = Q, 3 = H) 
    int ncDataCodeWord[4];	// �f�[�^�R�[�h���[�h��(���R�[�h���[�h�� - �q�r�R�[�h���[�h��)

    int ncAlignPoint;	// �A���C�����g�p�^�[�����W��
    int nAlignPoint[6];	// �A���C�����g�p�^�[�����S���W

    RS_BLOCKINFO RS_BlockInfo1[4]; // �q�r�u���b�N���(1)
    RS_BLOCKINFO RS_BlockInfo2[4]; // �q�r�u���b�N���(2)

} QR_VERSIONINFO, *LPQR_VERSIONINFO;


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode �N���X

class CQR_Encode
{
    // �\�z/����
public:
    CQR_Encode();
    ~CQR_Encode();

public:
    int m_nLevel;		// 
    int m_nVersion;		// 
    bool m_bAutoExtent;	// 
    int m_nMaskingNo;	// 

public:
    int m_nSymbleSize;
    unsigned char m_byModuleData[MAX_MODULESIZE][MAX_MODULESIZE]; // [x][y]
    // bit5:
    // bit4:
    // bit1:
    // bit0:
    // 20h

private:
    int m_ncDataCodeWordBit; // 
    unsigned char m_byDataCodeWord[MAX_DATACODEWORD]; // 

    int m_ncDataBlock;
    unsigned char m_byBlockMode[MAX_DATACODEWORD];
    int m_nBlockLength[MAX_DATACODEWORD];

    int m_ncAllCodeWord; //
    unsigned char m_byAllCodeWord[MAX_ALLCODEWORD]; // 
    unsigned char m_byRSWork[MAX_CODEBLOCK]; // 

    // 
public:
    bool EncodeData(int nLevel, int nVersion, bool bAutoExtent, int nMaskingNo,const char * lpsSource, int ncSource = 0);

private:
    int GetEncodeVersion(int nVersion,const char * lpsSource, int ncLength);
    bool EncodeSourceData(const char * lpsSource, int ncLength, int nVerGroup);

    int GetBitLength(unsigned char nMode, int ncData, int nVerGroup);

    int SetBitStream(int nIndex, unsigned short wData, int ncData);

    bool IsNumeralData(unsigned char c);
    bool IsAlphabetData(unsigned char c);
    bool IsKanjiData(unsigned char c1, unsigned char c2);

    unsigned char AlphabetToBinaly(unsigned char c);
    unsigned short KanjiToBinaly(unsigned short wc);

    void GetRSCodeWord(unsigned char* lpbyRSWork, int ncDataCodeWord, int ncRSCodeWord);

    // ���W���[���z�u�֘A�t�@���N�V����
private:
    void FormatModule();

    void SetFunctionModule();
    void SetFinderPattern(int x, int y);
    void SetAlignmentPattern(int x, int y);
    void SetVersionPattern();
    void SetCodeWordPattern();
    void SetMaskingPattern(int nPatternNo);
    void SetFormatInfoPattern(int nPatternNo);
    int  CountPenalty();

};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_QR_ENCODE_H__AC886DF7_C0AE_4C9F_AC7A_FCDA8CB1DD37__INCLUDED_)
