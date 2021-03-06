/**
 * \copyright
 * Copyright (c) 2012-2018, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 */

#include "BoundaryConditionBuilder.h"

#include "MeshGeoToolsLib/BoundaryElementsSearcher.h"
#include "MeshGeoToolsLib/MeshNodeSearcher.h"
#include "MeshGeoToolsLib/SearchLength.h"
#include "MeshLib/Mesh.h"
#include "ProcessLib/BoundaryCondition/BoundaryConditionConfig.h"

#include "NeumannBoundaryCondition.h"

namespace ProcessLib
{
namespace LIE
{
std::unique_ptr<BoundaryCondition>
BoundaryConditionBuilder::createNeumannBoundaryCondition(
    const BoundaryConditionConfig& config,
    const NumLib::LocalToGlobalIndexMap& dof_table, const MeshLib::Mesh& mesh,
    const int variable_id, const unsigned integration_order,
    const unsigned shapefunction_order,
    const std::vector<std::unique_ptr<ProcessLib::ParameterBase>>& parameters)
{
    auto search_length_strategy =
        std::make_unique<MeshGeoToolsLib::SearchLength>();
    MeshGeoToolsLib::MeshNodeSearcher const& mesh_node_searcher =
        MeshGeoToolsLib::MeshNodeSearcher::getMeshNodeSearcher(
            mesh, std::move(search_length_strategy));

    MeshGeoToolsLib::BoundaryElementsSearcher boundary_element_searcher(
        mesh, mesh_node_searcher);

    return ProcessLib::LIE::createNeumannBoundaryCondition(
        config.config,
        getClonedElements(boundary_element_searcher, config.geometry),
        dof_table, variable_id, *config.component_id, mesh.isAxiallySymmetric(),
        integration_order, shapefunction_order, mesh.getDimension(), parameters,
        _fracture_prop);
}

}  // namespace LIE
}  // namespace ProcessLib
