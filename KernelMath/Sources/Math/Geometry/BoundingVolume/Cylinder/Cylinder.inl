
#include INCL_KMATH(Math/Geometry/Quaternions/Quaternion)

//-----------------------------------------------------------------------------
// Name: CYLINDER::Build
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void CYLINDER::Build(const VECTOR &Center, 
					 const VECTOR &CylDir, 
					 const float rCylRadius, 
					 const float rCylHeight)
{
	_Center = Center;
	SetRadius(rCylRadius);
	SetHeight(rCylHeight);

	SetAxis(CylDir);
}

//-----------------------------------------------------------------------------
//	Name:		SetCenter
//	Object:		
//	02-04-09:	JLT - Created
//-----------------------------------------------------------------------------
void CYLINDER::SetCenter(const VECTOR &Pos)
{
	_Center = Pos;
}

//-----------------------------------------------------------------------------
//	Name:		SetHeight
//	Object:		
//	02-04-09:	JLT - Created
//-----------------------------------------------------------------------------
void CYLINDER::SetHeight(const float rHeight)
{
	_XDir.SetPad(rHeight);	
}

//-----------------------------------------------------------------------------
//	Name:		GetHeight
//	Object:		
//	02-04-09:	JLT - Created
//-----------------------------------------------------------------------------
float CYLINDER::GetHeight() const
{
	return (_XDir.GetPad());
}

//-----------------------------------------------------------------------------
//	Name:		SetRadius
//	Object:		
//	02-04-09:	JLT - Created
//-----------------------------------------------------------------------------
void CYLINDER::SetRadius(const float rRadius)
{
	_Center.SetPad(rRadius);
}

//-----------------------------------------------------------------------------
//	Name:		GetRadius
//	Object:		
//	02-04-09:	JLT - Created
//-----------------------------------------------------------------------------
float CYLINDER::GetRadius() const
{
	return (_Center.GetPad());
}

//-----------------------------------------------------------------------------
//	Name:		SetAxis
//	Object:		
//	02-04-09:	JLT - Created
//-----------------------------------------------------------------------------
void CYLINDER::SetAxis(const VECTOR &V)
{
	VECTOR vCylDir(V);
	vCylDir.Normalize();
	
	QUATERNION Q(_ZDir, vCylDir);
	
	_XDir = Q * _XDir;
	_YDir = Q * _YDir;
	
	QDT_ASSERT(_XDir.IsNormalized() && _YDir.IsNormalized());
	
	_ZDir.Copy(vCylDir);
}

//-----------------------------------------------------------------------------
//	Name:		GetAxis
//	Object:		
//	02-04-09:	JLT - Created
//-----------------------------------------------------------------------------
const VECTOR & CYLINDER::GetAxis() const
{
	return _ZDir;
}

//-----------------------------------------------------------------------------
//	Name:		GetXDir
//	Object:		
//	02-12-02:	JLT - Created
//-----------------------------------------------------------------------------
const VECTOR & CYLINDER::GetXDir() const
{
	return _XDir;
}

//-----------------------------------------------------------------------------
//	Name:		GetYDir
//	Object:		
//	02-12-02:	JLT - Created
//-----------------------------------------------------------------------------
const VECTOR & CYLINDER::GetYDir() const
{
	return _YDir;
}



