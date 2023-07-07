#include "Rule.h"
#include "Types.h"

// ***Base Rule implementation***

void Rule::setID(int ID){
    this->ID = ID;
}

void Rule::print(){
    throw std::runtime_error("Not implemented yet");
}

int& Rule::getID(){
    return ID;
}

double Rule::getAppliedConfidence(int nUnseen){
    return (double) cpredicted/((double) predicted + (double)nUnseen); 
}

double Rule::getConfidence(){
    return (double) cpredicted/(double) predicted; 
}

std::string Rule::getRuleString(){
    throw std::runtime_error("Not implemented yet");
}
long long Rule::getBodyHash(){
    throw std::runtime_error("Not implemented yet");

}
void Rule::computeBodyHash(){
    throw std::runtime_error("Not implemented yet");

}
int Rule::getTargetRel(){
   return targetRel;

}

// ***RuleB implementation*** 

RuleB::RuleB(std::vector<int> &relations, std::vector<bool> &directions) {
    if(relations.size() != (directions.size() + 1)) {
        throw std::invalid_argument("'Directions' size should be one less than 'relations' size in construction of RuleB");
    }
    if(relations.size() < 2) {
        throw std::invalid_argument("Cannot construct a RuleB with no body atom.");
    }		
	this->relations = relations;
    this->directions = directions;	
    this->targetRel = relations.front();   
}

// TODO: dont return, just add predictions to a passed data structure
// or not return a copy
std::vector<std::vector<int>> RuleB::materialize(TripleStorage& triples){

    std::vector<std::vector<int>> predictions;

    RelNodeToNodes* relNtoN = nullptr;
     // First body atom is (v1,v2)
    if (directions[0]){
         relNtoN =  &triples.getRelHeadToTails();
    // First body atom is (v2,v1)    
    }else{
         relNtoN =  &triples.getRelTailToHeads();
    }
    auto it = (*relNtoN).find(relations[1]);
    if (!(it==(*relNtoN).end())){
        NodeToNodes& NtoN = it->second;
         // every entity e that satisfies b1(e,someY) [or b1(someX, e)]
         for (auto const& pair: NtoN){
                const int& e = pair.first;
                Nodes closingEntities;
                std::set<int> substitutions = {e};
                searchCurrGroundings(1, e, substitutions, triples, closingEntities);
                for (const int& cEnt:  closingEntities){
                    std::vector<int> pred = {e, targetRel, cEnt};
                    predictions.push_back(pred);
                }
            }
    }
    return predictions;

}

void RuleB::searchCurrGroundings(
			int currAtomIdx, int currEntity, std::set<int>& substitutions, TripleStorage& triples, Nodes& closingEntities
		)
{
    Nodes* nextEntities = nullptr;
    int currRel = relations[currAtomIdx];
    RelNodeToNodes& relNtoN = directions[currAtomIdx-1] ? triples.getRelHeadToTails() : triples.getRelTailToHeads();
    auto it = relNtoN.find(relations[currAtomIdx]);
    if (!(it==relNtoN.end())){
        NodeToNodes& NtoN = it->second;
        auto entIt = NtoN.find(currEntity);
        if (!(entIt==NtoN.end())){
            nextEntities = &(entIt->second);

            if (currAtomIdx == relations.size()-1){
                //copies
                for(int ent: *nextEntities){
                    // respect object identity constraint
                    if (substitutions.find(ent)==substitutions.end()){
                        closingEntities.insert(ent);
                    }
                }
            }else{
                for(int ent: *nextEntities){
                    if (substitutions.find(ent)==substitutions.end()){
                        substitutions.insert(ent);
                        searchCurrGroundings(currAtomIdx+1, ent, substitutions, triples, closingEntities);
                    }
                }
            }
        }
    } 
}


