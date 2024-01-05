#include "Index.h"

void Index::addNode(std::string& nodesstring) {
	if (nodeToId.find(nodesstring) == nodeToId.end()) {
		nodeToId[nodesstring] = maxNodeID;
		idToNode[maxNodeID] = nodesstring;
		maxNodeID++;
	}
}

void Index::addRelation(std::string& relstring) {
	if (relToId.find(relstring) == relToId.end()) {
		relToId[relstring] = maxRelID;
		idToRel[maxRelID] = relstring;
		maxRelID++;
	}
}

int Index::getNodeSize() {
	return nodeToId.size();
}

int Index::getRelSize() {
	return relToId.size();
}

int Index::getIdOfNodestring(std::string& node) {
	auto it = nodeToId.find(node);
	if (it != nodeToId.end()) {
		return it->second;
	}
	else {
		//if(Properties::get().PREDICT_UNKNOWN){
		//	return getIdOfNodestring(Properties::get().UNK_TOKEN);
		//} else {
			throw std::runtime_error(("Error: Node " + node + " not found in Trainingset").c_str());
		//}
	}
}

std::string Index::getStringOfNodeId(int& id) {
	auto it = idToNode.find(id);
	if (it != idToNode.end()) {
		return it->second;
	}
	else {
		std::cout << "Error: Idx " << id << " not found in idxToNode set";
		exit(-1);
	}
}

int Index::getIdOfRelationstring(std::string& relation) {
	auto it = relToId.find(relation);
	if (it != relToId.end()) {
		return it->second;
	}
	else {
		throw std::runtime_error(("Error: Relation " + relation + " not found in index"));
	}
}

std::string Index::getStringOfRelId(int& id) {
	auto it = idToRel.find(id);
	if (it != idToRel.end()) {
		return it->second;
	}
	else {
		std::cout << "Error: Idx " << id << " not found in idxToRel set";
		exit(-1);
	}
}

void Index::rehash() {
	nodeToId.rehash(nodeToId.size());
	relToId.rehash(relToId.size());
	idToNode.rehash(idToNode.size());
	idToRel.rehash(idToRel.size());
}


std::unordered_map<std::string, int>& Index::getNodeToIdx(){
	return nodeToId;
}

std::unordered_map<std::string, int>& Index::getRelationToIdx(){
	return relToId;

}