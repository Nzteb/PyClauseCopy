#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "src/cpp/api/Api.h"
#include "src/cpp/api/RulesHandler.h"
#include "src/cpp/api/RankingHandler.h"
#include "src/cpp/api/QAHandler.h"
#include "src/cpp/api/DataHandler.h"
#include "src/cpp/api/PredictionHandler.h"
#include <string>
#include <array>

// *Example* 
int add(int i, int j) {
    return i + j;
}

namespace py = pybind11;


//***PyClause backend bindings + examples *****
PYBIND11_MODULE(c_clause, m) {

    //** example bindings **
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: python_example

        .. autosummary::
           :toctree: _generate

           add
           subtract
    )pbdoc";

    m.def("add", &add, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");

    m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

    // **backend bindings**
    py::class_<RankingHandler>(m, "RankingHandler") 
        .def(py::init<std::map<std::string, std::string>>())
        .def("calc_ranking", &RankingHandler::calculateRanking)
        .def("get_ranking", &RankingHandler::getRanking)
        .def("write_ranking", &RankingHandler::writeRanking)
    ; //class end

    py::class_<QAHandler>(m, "QAHandler") 
        .def(py::init<std::map<std::string, std::string>>())
        .def("calculate_answers", py::overload_cast<std::vector<std::pair<int, int>>, std::shared_ptr<DataHandler>, std::string>(&QAHandler::calculate_answers))
        .def("calculate_answers", py::overload_cast<std::vector<std::pair<std::string, std::string>>, std::shared_ptr<DataHandler>, std::string>(&QAHandler::calculate_answers))
        .def(
            "get_answers",
            [](QAHandler& self, bool return_strings)->py::object{
                if (return_strings){
                    return py::cast(self.getStrAnswers());
                }else{
                    return py::cast(self.getIdxAnswers());
                }
            }
        )
        .def("set_options", &QAHandler::setOptions)
    ; //class end

    py::class_<RulesHandler>(m, "RulesHandler") 
        .def(py::init<>())
        .def(
            "stats_and_predictions", &RulesHandler::calcRulesPredictions,
            R"pbdoc(
                Given a string rule calculates all stats and predictions of rules. 
                args: string:rulestring; bool returnPredictions: if true return predictions; bool returnStats: if true returns exact stats.
                returns: tuple where tuple[0] are the predictions , tuple[1][0] are number of exact predictions tuple[1][1] number of true predictions.

            )pbdoc" 
        )
    ; //class end

    py::class_<DataHandler,  std::shared_ptr<DataHandler>>(m, "DataHandler") 
        .def(py::init<std::map<std::string, std::string>>())
        .def("load_rules", py::overload_cast<std::string>(&DataHandler::loadRules))
        .def("load_rules", py::overload_cast<std::vector<std::string>>(&DataHandler::loadRules))
        .def("load_data", py::overload_cast<std::string>(&DataHandler::loadData))
        .def("load_data", py::overload_cast<std::string, std::string>(&DataHandler::loadData))
        .def("load_data", py::overload_cast<std::string, std::string, std::string>(&DataHandler::loadData))
        .def("load_data", py::overload_cast<std::vector<std::array<int, 3>>>(&DataHandler::loadData))
        .def("load_data", py::overload_cast<std::vector<std::array<int, 3>>, std::vector<std::array<int, 3>>>(&DataHandler::loadData))
        .def("load_data", py::overload_cast<std::vector<std::array<int, 3>>, std::vector<std::array<int, 3>>, std::vector<std::array<int, 3>>>(&DataHandler::loadData))
        .def("load_data", py::overload_cast<std::vector<std::array<std::string, 3>>>(&DataHandler::loadData))
        .def("load_data", py::overload_cast<std::vector<std::array<std::string, 3>>, std::vector<std::array<std::string, 3>>>(&DataHandler::loadData))
        .def("load_data", py::overload_cast<std::vector<std::array<std::string, 3>>, std::vector<std::array<std::string, 3>>, std::vector<std::array<std::string, 3>>>(&DataHandler::loadData))
        .def("entity_map", &DataHandler::getNodeToIdx)
        .def("relation_map", &DataHandler::getRelationToIdx)
        .def("replace_ent_tokens", &DataHandler::subsEntityStrings)
        .def("replace_rel_tokens", &DataHandler::subsRelationStrings)
        .def("set_entity_index",  &DataHandler::setNodeIndex)
        .def("set_relation_index",  &DataHandler::setRelIndex)
        .def("rule_index",  &DataHandler::getRuleIdx)
    ; // class end


    py::class_<PredictionHandler>(m, "PredictionHandler") 
        .def(py::init<std::map<std::string, std::string>>())
        .def("score_triples", py::overload_cast<std::string, std::shared_ptr<DataHandler>>(&PredictionHandler::scoreTriples),
               R"pbdoc(
                    Takes as input np.array/list of idx's or a list of string/token triples (tuples or lists)
                    or a path to a file containing tab separarated string/token triples. Entities and relation tokens must
                    be known, e.g., they must have be loaded in some triples in the DataLoader.
                )pbdoc"
            ) 
        .def("score_triples", py::overload_cast<std::vector<std::array<int,3>>, std::shared_ptr<DataHandler>>(&PredictionHandler::scoreTriples))
        .def("score_triples", py::overload_cast<std::vector<std::array<std::string,3>>, std::shared_ptr<DataHandler>>(&PredictionHandler::scoreTriples))
        .def(
            "get_scores",
            [](PredictionHandler& self, bool return_strings)->py::object{
                        if (return_strings){
                            return py::cast(self.getStrScores()); //TODO kinda spooky why this works, getStrScores is no python function binding
                        }else{
                            return py::cast(self.getIdxScores());
                        }
                    }
        )
        .def("get_explanations", &PredictionHandler::getStrExplanations)
        .def(
            "get_explanations",
            [](PredictionHandler& self, bool return_strings)->py::object{
                        if (return_strings){
                            return py::cast(self.getStrExplanations());
                        }else{
                            return py::cast(self.getIdxExplanations());
                        }
                    }
        )        
    ; // class end
}


