/**************************************************************************/
/*  Moskito - THMC sImulator for GEoscience Research                        */
/*                                                                        */
/*  Copyright (C) 2017 by Maziar Gholami Korzani                          */
/*  Karlsruhe Institute of Technology, Institute of Applied Geosciences   */
/*  Division of Geothermal Research                                       */
/*                                                                        */
/*  This file is part of Moskito App                                        */
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

#include "MoskitoWaterConst.h"

registerMooseObject("FluidPropertiesApp", MoskitoWaterConst);

InputParameters
MoskitoWaterConst::validParams()
{
  InputParameters params = SinglePhaseFluidProperties::validParams();
  params.addParam<Real>("molar_mass", 1.8E-2,
        "Constant molar mass of the fluid (kg/mol)");
  params.addParam<Real>("thermal_expansion", 2.14E-4,
        "Constant coefficient of thermal expansion (1/K)");
  params.addParam<Real>("cv", 4186.0,
        "Constant specific heat capacity at constant volume (J/kg/K)");
  params.addParam<Real>("cp", 4194.0,
        "Constant specific heat capacity at constant pressure (J/kg/K)");
  params.addRangeCheckedParam<Real>("bulk_modulus", 2.0E9,
        "bulk_modulus>0", "Constant bulk modulus (Pa)");
  params.addParam<Real>("thermal_conductivity", 0.6,
        "Constant thermal conductivity (W/m/K)");
  params.addParam<Real>("specific_entropy", 300.0,
        "Constant specific entropy (J/kg/K)");
  params.addParam<Real>("porepressure_coefficient", 1.0,
        "The enthalpy is internal_energy + P / density * "
        "porepressure_coefficient.  Physically this should be 1.0, "
        "but analytic solutions are simplified when it is zero");
  params.addParam<Real>("viscosity", 1.0E-3,
        "Constant dynamic viscosity (Pa.s)");
  params.addParam<Real>("density", 1000.0, "Constant density");
  params.addClassDescription("Fluid properties for a simple fluid with"
        " a constan density and viscosity");
  return params;
}

MoskitoWaterConst::MoskitoWaterConst(const InputParameters & parameters)
  : SinglePhaseFluidProperties(parameters),
    _molar_mass(getParam<Real>("molar_mass")),
    _thermal_expansion(getParam<Real>("thermal_expansion")),
    _cv(getParam<Real>("cv")),
    _cp(getParam<Real>("cp")),
    _bulk_modulus(getParam<Real>("bulk_modulus")),
    _thermal_conductivity(getParam<Real>("thermal_conductivity")),
    _specific_entropy(getParam<Real>("specific_entropy")),
    _pp_coeff(getParam<Real>("porepressure_coefficient")),
    _viscosity(getParam<Real>("viscosity")),
    _density(getParam<Real>("density"))
{
}

MoskitoWaterConst::~MoskitoWaterConst() {}

std::string
MoskitoWaterConst::fluidName() const
{
  return "Constant Water properties";
}

Real
MoskitoWaterConst::molarMass() const
{
  return _molar_mass;
}

Real MoskitoWaterConst::beta_from_p_T(Real /*pressure*/, Real /*temperature*/) const
{
  return _thermal_expansion;
}

Real MoskitoWaterConst::cp_from_p_T(Real /*pressure*/, Real /*temperature*/) const { return _cp; }

Real MoskitoWaterConst::cv_from_p_T(Real /*pressure*/, Real /*temperature*/) const { return _cv; }

Real
MoskitoWaterConst::c_from_p_T(Real pressure, Real temperature) const
{
  return std::sqrt(_bulk_modulus / rho_from_p_T(pressure, temperature));
}

Real MoskitoWaterConst::k_from_p_T(Real /*pressure*/, Real /*temperature*/) const
{
  return _thermal_conductivity;
}

void
MoskitoWaterConst::k_from_p_T(
    Real /*pressure*/, Real /*temperature*/, Real & k, Real & dk_dp, Real & dk_dT) const
{
  k = _thermal_conductivity;
  dk_dp = 0;
  dk_dT = 0;
}

Real MoskitoWaterConst::s_from_p_T(Real /*pressure*/, Real /*temperature*/) const
{
  return _specific_entropy;
}

Real
MoskitoWaterConst::rho_from_p_T(Real /*pressure*/, Real /*temperature*/) const
{
  return _density;
}

void
MoskitoWaterConst::rho_from_p_T(
    Real pressure, Real temperature, Real & rho, Real & drho_dp, Real & drho_dT) const
{
  rho = this->rho_from_p_T(pressure, temperature);
  drho_dp = 0.0;
  drho_dT = 0.0;
}

Real
MoskitoWaterConst::e_from_p_T(Real /*pressure*/, Real temperature) const
{
  return _cv * temperature;
}

void
MoskitoWaterConst::e_from_p_T(
    Real pressure, Real temperature, Real & e, Real & de_dp, Real & de_dT) const
{
  e = this->e_from_p_T(pressure, temperature);
  de_dp = 0.0;
  de_dT = _cv;
}

Real MoskitoWaterConst::mu_from_p_T(Real /*pressure*/, Real temperature) const
{
  return _viscosity;
}

void
MoskitoWaterConst::mu_from_p_T(
    Real pressure, Real temperature, Real & mu, Real & dmu_dp, Real & dmu_dT) const
{
  mu = this->mu_from_p_T(pressure, temperature);
  dmu_dp = 0.0;
  dmu_dT = 0.0;
}

Real
MoskitoWaterConst::h_from_p_T(Real pressure, Real temperature) const
{
  return e_from_p_T(pressure, temperature) + _pp_coeff * pressure / rho_from_p_T(pressure, temperature);
}

void
MoskitoWaterConst::h_from_p_T(
    Real pressure, Real temperature, Real & h, Real & dh_dp, Real & dh_dT) const
{
  h = this->h_from_p_T(pressure, temperature);

  Real density, ddensity_dp, ddensity_dT;
  rho_from_p_T(pressure, temperature, density, ddensity_dp, ddensity_dT);

  dh_dp = _pp_coeff / density - _pp_coeff * pressure * ddensity_dp / density / density;
  dh_dT = _cv - _pp_coeff * pressure * ddensity_dT / density / density;
}
