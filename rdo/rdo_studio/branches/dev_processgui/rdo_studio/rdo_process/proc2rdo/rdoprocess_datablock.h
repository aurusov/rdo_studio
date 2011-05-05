/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdoprocess_datablock.h
 * author   : Шувалов Андрей
 * date     : 04.05.2011
 * bref     : 
 * indent   : 4T
 */

#ifndef RDO_PROCESS_DATABLOCK_H
#define RDO_PROCESS_DATABLOCK_H

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

// ----------------------------------------------------------------------------
// ---------- RPShapeDataBlock
// ----------------------------------------------------------------------------
class RPShapeDataBlock
{
public:

	enum ZakonRaspr
	{
		Const,
		Normal,
		Uniform,
		Exp
	};

	RPShapeDataBlock(ZakonRaspr zakon, tstring name);

	void setBase  (int base   );
	void setAmount(int amount );
	void setDisp  (double disp);
	void setExp   (double exp );

	ZakonRaspr getZakon() {return m_zakon;}
	tstring    getName () {return m_name; }
	int        getBase () {return m_base; }
	double     getDisp () {return m_disp; }
	double     getExp  () {return m_exp;  }

protected:

	ZakonRaspr m_zakon;
	tstring    m_name;
	int        m_base;
	double     m_exp;
	double     m_disp;
};

// ----------------------------------------------------------------------------
// ---------- RPShapeDataBlockCreate
// ----------------------------------------------------------------------------
class RPShapeDataBlockCreate: public RPShapeDataBlock
{
public:

	RPShapeDataBlockCreate(RPShapeDataBlock::ZakonRaspr zakon, tstring name);
	~RPShapeDataBlockCreate();

	void setAmount(int amount);

	int getAmount() const {return m_amount;}

//	RPShapeDataBlock::ZakonRaspr getZakon() const {return m_zakon;}

private:

	int m_amount;

};
#endif // RDO_PROCESS_DATABLOCK_H
