/*------------------------------------------------------------------------*/
/*  Copyright 2014 Sandia Corporation.                                    */
/*  This software is released under the license detailed                  */
/*  in the file, LICENSE, which is located in the top-level Nalu          */
/*  directory structure                                                   */
/*------------------------------------------------------------------------*/

#ifndef ContinuityGclElemKernel_H
#define ContinuityGclElemKernel_H

#include "kernel/Kernel.h"
#include "FieldTypeDef.h"

#include <stk_mesh/base/BulkData.hpp>
#include <stk_mesh/base/Entity.hpp>

#include <Kokkos_Core.hpp>

namespace sierra {
namespace nalu {

class TimeIntegrator;
class SolutionOptions;
class MasterElement;
class ElemDataRequests;

/** GCL for continuity equation (pressure DOF)
 */
template<typename AlgTraits>
class ContinuityGclElemKernel: public Kernel
{
public:
  ContinuityGclElemKernel(
    const stk::mesh::BulkData&,
    const SolutionOptions&,
    ElemDataRequests&,
    const bool);

  virtual ~ContinuityGclElemKernel();

  /** Perform pre-timestep work for the computational kernel
   */
  virtual void setup(const TimeIntegrator&);

  /** Execute the kernel within a Kokkos loop and populate the LHS and RHS for
   *  the linear solve
   */
  virtual void execute(
    SharedMemView<DoubleType**>&,
    SharedMemView<DoubleType*>&,
    ScratchViews<DoubleType>&);

private:
  ContinuityGclElemKernel() = delete;

  ScalarFieldType *densityNp1_{nullptr};
  ScalarFieldType *divV_{nullptr};
  VectorFieldType *coordinates_{nullptr};

  double dt_{0.0};
  double gamma1_{0.0};
  const bool lumpedMass_;

  // inverse density scaling tied to balancedForce_
  const double densFac_;
  const double om_densFac_;

  /// Integration point to node mapping
  const int* ipNodeMap_;

  /// Shape functions
  AlignedViewType<DoubleType[AlgTraits::numScvIp_][AlgTraits::nodesPerElement_]> v_shape_function_ {"view_shape_func"};
};

}  // nalu
}  // sierra

#endif /* ContinuityGclElemKernel_H */
