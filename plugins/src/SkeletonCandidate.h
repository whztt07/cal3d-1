//----------------------------------------------------------------------------//
// SkeletonCandidate.h                                                        //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef SKELETON_CANDIDATE_H
#define SKELETON_CANDIDATE_H

class CBoneCandidate;
class CBaseNode;
class CalCoreSkeleton;
class CalCoreBone;

class CSkeletonCandidate
{
protected:
	std::vector<CBoneCandidate *> m_vectorBoneCandidate;
	std::list<int> m_listRootBoneCandidateId;
	std::string m_strFilename;
        boost::shared_ptr<CalCoreSkeleton> m_skeleton;

public:
	CSkeletonCandidate();
	virtual ~CSkeletonCandidate();

	int BuildSelectedId();
	void Clear();
	bool CreateFromInterface();

	bool CreateFromInterfaceFromMaxScriptCall();

	bool CreateFromSkeletonFile(const std::string& strFilename);
	int GetBoneId(const std::string& strName);
	const std::string& GetFilename();
	std::list<int> GetListRootBoneCandidateId();
	int GetParentSelectedId(int boneCandidateId);
	int GetSelectedCount();
        CalCoreSkeleton * GetCoreSkeleton() { return m_skeleton.get(); }
	void GetTranslationAndRotation(int boneCandidateId, float time, CalVector& translation, CalQuaternion& rotation);
	void GetTranslationAndRotationBoneSpace(int boneCandidateId, float time, CalVector& translation, CalQuaternion& rotation);
	std::vector<CBoneCandidate *> GetVectorBoneCandidate();

protected:
	bool AddNode(CBaseNode *pNode, int parentId);
	bool AddNode(CalCoreSkeleton *pCoreSkeleton, CalCoreBone *pCoreBone, int parentId);
};

#endif

//----------------------------------------------------------------------------//
