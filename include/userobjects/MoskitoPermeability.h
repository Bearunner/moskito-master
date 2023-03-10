/**************************************************************************/
/*  MOSKITO - THMC sImulator for GEoscience Research                        */
/*                                                                        */
/*  Copyright (C) 2017 by Maziar Gholami Korzani, Robert Egert            */
/*  Karlsruhe Institute of Technology, Institute of Applied Geosciences   */
/*  Division of Geothermal Research                                       */
/*                                                                        */
/*  This file is part of MOSKITO App                                        */
/*                                                                        */
/*  This program is free software: you can redistribute it and/or modify  */
/*  it under the terms of the GNU General Public License as published by  */
/*  the Free Software Foundation, either version 3 of the License, or     */
/*  (at your option) any later version.                                   */
/*                                                                        */
/*  This program is distributed in the hope that it will be useful,       */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          */
/*  GNU General Public License for more details.                          */
/*                                                                        */
/*  You should have received a copy of the GNU General Public License     */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>  */
/**************************************************************************/

#pragma once

#include "GeneralUserObject.h"
#include "RankTwoTensor.h"

class MoskitoPermeability : public GeneralUserObject
{
public:
  static InputParameters validParams();

  MoskitoPermeability(const InputParameters & parameters);

  virtual void execute();
  virtual void initialize();
  virtual void finalize();

  /// permeability matrix (m^2); called from Material
  virtual RankTwoTensor Permeability(const int & dim, const Real & porosity, const Real & scale_factor, const std::vector<Real> kmat) const = 0;

protected:
  // Creates the permeability tensor as function of input and dimension
  virtual RankTwoTensor PermeabilityTensorCalculator(const int & dim, const std::vector<Real> & k0, const MooseEnum & _permeability_type) const;

  enum PT {isotropic, orthotropic, anisotropic};
};
