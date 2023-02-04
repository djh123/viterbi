#include "Viterbi.h"
#include <assert.h>
#include<iostream>

#define ID_BOS 0
#define ID_EOS 1
#define MIN_VALUE -2000000000
void ViterbiNodeNet::GetBestPath() {
/*	ViterbiNode* pVbBestNode = NULL;
	for(int i = 0;i <= ;i--) {
		
	}*/
}




void ViterbiNodeNet::AllocBuff(const int iLenSen) {

	if(iLenSen <= 0) {
		return ;
	}
	if(!m_pClassResult) {
		m_pClassResult = new Result();
	}
	ClearNet();
	m_pClassResult->Alloc(iLenSen - 2);// 没有 <EOS><BOS>
	m_vbNodeMap = new ViterbiNode[iLenSen * MAX_UNIT_INDEX + 1];
	m_iWordCount = iLenSen;

}

bool ViterbiNodeNet::InitVbNodeNet( WordMap* pWordMap) {
	m_iWordCount = pWordMap->GetColCount() ;
	if(pWordMap == NULL || m_iWordCount == 0) {
		return false;
	}

	if(m_pClassResult) {
		m_pClassResult->Clear();
	}
	m_WordUnitList.clear();

	AllocBuff(m_iWordCount / 2 + 1 ); // 因为<EOS><BOS> 只占了两列，所以要 + 1


//	fprintf(stderr,"begin InitVbNodeNet!\n");
	//m_iWordCount = pWordMap->clomn_len ;
	for(int i = 0; i < m_iWordCount;i++) {
		ClomnInfo* pColInfo  = (ClomnInfo*)(pWordMap->GetColInfo(i));
		if(!pColInfo) {
			continue;
		}
		m_iRowSize[i] = pColInfo->GetWordNum();
		for(int j = 0; j < m_iRowSize[i];j++) {
			const WordUnit* pWordUnit = pColInfo->GetWordUnit(j);
			if(!pWordUnit) {
				continue;
			}
			ViterbiNode* pViterbiNode = GetViterbiNode(i,j);
			pViterbiNode->dScore = -2000000000;
			pViterbiNode->iWordId = pWordUnit->word_id;
			pViterbiNode->dFreq = pWordUnit->dFreq;
			pViterbiNode->dRollBack = pWordUnit->dRollBack;
			pViterbiNode->nodeTag = (NodeTag)pWordUnit->word_pos_type;
			pViterbiNode->iWordLen = pWordUnit->word_len;
			pViterbiNode->pUcsChar = pWordUnit->pUcsChar;
		}
	} 
//	 fprintf(stderr,"end InitVbNodeNet!\n");
	return true;
}


int ViterbiNodeNet::ForiPersonTag(const RoleShiftDict* pRoleShiftDict,const RoleTagModel* pRollMdl) {
	if(pRoleShiftDict == NULL || pRollMdl == NULL || m_pRecPerName == NULL) {
		return -1;
	}
//	m_pRecPerName->SetRecogType(REG_FORI_NAME);
	return m_pForPerName->Mark(m_pClassResult,pRoleShiftDict,pRollMdl);
//	return m_pRecPerName->Mark(m_pClassResult,pRoleShiftDict,pRollMdl);
}
int ViterbiNodeNet::CHSPersonTag(const RoleShiftDict* pRoleShiftDict,const RoleTagModel* pRollMdl) {
	if(pRoleShiftDict == NULL || pRollMdl == NULL || m_pRecPerName == NULL) {
		return -1;
	}

	m_pRecPerName->SetRecogType(REG_CHS_NAME);
	return m_pRecPerName->Mark(m_pClassResult,pRoleShiftDict,pRollMdl);
}

int ViterbiNodeNet::PlaceTag(const RoleShiftDict* pRoleShiftDict,const RoleTagModel* pRollMdl) {
	if(pRoleShiftDict == NULL || pRollMdl == NULL || m_pRecPerName == NULL) {
		return -1;
	}
	return m_pRecPlace->Mark(m_pClassResult,pRoleShiftDict,pRollMdl);
}

int ViterbiNodeNet::GetBestSegment(const NGRramLMReader* pLMReader,const RoleShiftDict* m_pRoleShiftDict) {
	if(m_iWordCount <= 2 || pLMReader == NULL) {
		fprintf(stderr," 词语的个数不能小于等于0.\n");		
		return -1; // 词语的个数不能小于等于0.
	}
///	fprintf(stderr,"WordCount:%d\n",m_iWordCount);
	ViterbiNode* pVbBestEndNode = NULL;
	for(int i = 0; i < m_iWordCount  ;i++) {

		int ids[2];// 二元模型。
		double dTrans[2];
		if(i == 0) {
			//ViterbiNode* pViterbiNode = GetViterbiNode(i + 1,0);
			for(int m = 0; m < m_iRowSize[i + 1];m++) {
				ViterbiNode* pViterbiNode = GetViterbiNode(i + 1,m);
				ViterbiNode* pViterbiNodeBOS = GetViterbiNode( i ,0);
				assert(pViterbiNodeBOS != NULL);
				//pViterbiNode->dScore = pViterbiNode->dFreq;
				ids[0] = pViterbiNodeBOS->iWordId;
				ids[1] =  pViterbiNode->iWordId;
				int iIndex =  ((NGRramLMReader*)pLMReader)->SearchTransfer(ids,2,dTrans,2);
				double dFreq = 0.0;
				if(iIndex != -1) {
					dFreq = dTrans[0] ;
				} else {
					dFreq = pViterbiNodeBOS->dRollBack + pViterbiNode->dFreq;
				}
				  pViterbiNode->dScore = dFreq;
		//		pViterbiNode->dScore = pViterbiNode->dFreq;
			}
			continue;
		}
		
		for(int j = 0; j < m_iRowSize[i] ;j++) {
			//fprintf(stderr,"%d\t%d\t%f\n",i,j,m_vbNodeMap[i][j].dFreq);	
			ViterbiNode* pViterbiNode = GetViterbiNode(i,j);

			//如果该节点不是终止节点，则需要计算其子节点的value
		//	int ids[2];// 二元模型。
			ids[0] = pViterbiNode->iWordId ;
			int iCldCol = i + pViterbiNode->iWordLen;
			if(iCldCol < m_iWordCount) {
				for(int k = 0; k < m_iRowSize[iCldCol]; k++) {
					ViterbiNode* pVitbiTmp = GetViterbiNode(iCldCol,k);
					ids[1] = pVitbiTmp->iWordId;
					//从语言模型得到转移，重新计算值。
				//	double dTrans[2];
					int iIndex = ((NGRramLMReader*)pLMReader)->SearchTransfer(ids,2,dTrans,2);
					assert(iCldCol > i);
					if(iIndex != -1) {
						double dFreq = pViterbiNode->dScore  + dTrans[0] ;
						//	fprintf(stderr,"Trans:%d\t%d\t%f\n",i,j,dTrans[0]);  
						if(pVitbiTmp->dScore < dFreq) {
							pVitbiTmp->dScore = dFreq;
							pVitbiTmp->dFreqTransPre = dTrans[0];
							pVitbiTmp->pPreVbNode = pViterbiNode;
						}
					} else {
						double dFreq = pViterbiNode->dScore + pViterbiNode->dRollBack + pVitbiTmp->dFreq;
						if(pVitbiTmp->dScore < (dFreq )) {
							pVitbiTmp->dScore = dFreq ;
							pVitbiTmp->pPreVbNode = pViterbiNode;
							pVitbiTmp->dFreqTransPre = MIN_TRANS_VALUE;
						}
					}

				}

				

			}
		}
	}

//	fprintf(stderr,"Get The Best Result Success!\n");
	//如下为得到最佳的切分句子
	pVbBestEndNode = GetViterbiNode(m_iWordCount - 1,0)->pPreVbNode;
	while(pVbBestEndNode) {

	/*	WordUnitInfo wordInfo;
		wordInfo.iWordLen = pVbBestEndNode->iWordLen;
		wordInfo.iWordId = pVbBestEndNode->iWordId;
		wordInfo.pUcsChar = pVbBestEndNode->pUcsChar;*/
		m_WordUnitList.insert(m_WordUnitList.begin(),pVbBestEndNode);
		//m_vtBestPath.insert(m_vtBestPath.begin(),1,pVbBestEndNode);
		pVbBestEndNode = pVbBestEndNode->pPreVbNode;
	}

//	 fprintf(stderr,"m_WordUnitList:%d",m_WordUnitList.size());
	RuleRecog(SimpleRecog::GetInstance());
	int iOffset = 0;
	list< ViterbiNode* >::iterator iter = m_WordUnitList.begin();
//	fprintf(stderr,"m_WordUnitList:%d",m_WordUnitList.size());
	list< ViterbiNode* >::iterator iterNext = iter;
	for(; iter !=  m_WordUnitList.end();iter++) {
		ResultNode node;
		iterNext++;
		if(iterNext != m_WordUnitList.end()) { 
		//	fprintf(stderr,"%f\n",node.dFreqTransNext);
			node.dFreqTransNext = (*iterNext)->dFreqTransPre;
		}
		node.pUcsChar = (*iter)->pUcsChar;
		node.iWordLen = (*iter)->iWordLen;
		node.iWordId = (*iter)->iWordId;	
		node.tPosTag = (*iter)->tWordTag;
		m_pClassResult->AddResultNode(node);
	}

	

	//fprintf(stderr,"Best Segment Path Len:%d\n",m_vtBestPath.size());

	return 0;
}



void ViterbiNodeNet::PrintBestPath() {
	WordUnitIter iter = m_WordUnitList.begin();
	cout << "Best Path" << endl;
	for(;iter != m_WordUnitList.end();iter++) {
		int kk = (*iter)->iWordLen;
		cout << kk << " ";
	}
	cout  << "\n";
}

int ViterbiNodeNet::RuleRecog(SimpleRecog* pSimpleRecog) {
	if(pSimpleRecog == NULL) {
		return -1;
	}
	pSimpleRecog->RuleRecog(m_WordUnitList);

}

Viterbi::Viterbi(void)
{
	
}

Viterbi::~Viterbi(void)
{
	
}
