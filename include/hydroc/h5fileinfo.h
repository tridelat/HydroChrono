#pragma once

#include <string>
#include <vector>


namespace H5 {
	class H5File;
}

#include <chrono/core/ChMatrix.h>

/**@brief Extract bemio formated hdf5 data
 *
 * https://wec-sim.github.io/bemio/_modules/bemio/io/output.html
*/
class H5FileInfo {
public:
	bool printed = false;

	H5FileInfo(std::string file, std::string body_name);
    H5FileInfo() = delete;

	H5FileInfo(const H5FileInfo& old) = default;
	H5FileInfo& operator = (const H5FileInfo& rhs) = default;

    H5FileInfo(H5FileInfo&&) = default;
	H5FileInfo& operator = (H5FileInfo&& rhs) = default;

	~H5FileInfo();

	chrono::ChMatrixDynamic<double> GetInfAddedMassMatrix() const;
	double GetHydrostaticStiffness(int i, int j) const;
	double GetRIRFval(int i, int n, int m) const;
    double GetExcitationIRFval(int i, int n, int m) const;
	int GetRIRFDims(int i) const;
    int GetExcitationIRFDims(int i) const;
	std::vector<double> GetRIRFTimeVector() const;
    std::vector<double> GetExcitationIRFTime() const;
    Eigen::VectorXd GetExcitationIRFTimeResampled() const;
	double GetExcitationMagValue(int m, int n, int w) const;
	double GetExcitationMagInterp(int i, int j, double freq_index_des) const;
	double GetOmegaDelta() const;
	double GetOmegaMax() const;
	double GetExcitationPhaseValue(int m, int n, int w) const;
	double GetExcitationPhaseInterp(int i, int j, double freq_index_des) const;
	double GetNumFreqs() const;

	std::vector<double> cg;
	std::vector<double> cb;
	double GetRho() const { return _rho;}
	double GetDispVol() const  { return _disp_vol;}

	std::pair<Eigen::VectorXd, Eigen::VectorXd> ResampleExcitationIRF(double dt_new);
    void ResampleExcitationIRFTime(double dt_new);

	int bodyNum;
	chrono::ChMatrixDynamic<double> lin_matrix;
private:

	double _rho;
	double _g;
	double _disp_vol;
	double _rirf_timestep;
	std::vector<double> freq_list;
	chrono::ChMatrixDynamic<double> inf_added_mass;
	std::vector<double> rirf_matrix;
	std::vector<int> rirf_dims;
	std::vector<double> rirf_time_vector;
	std::vector<double> radiation_damping_matrix; // TODO check about names
	std::vector<int> Bw_dims; // TODO check with dave on name for dimensions of radiation damping matrix
	std::vector<double> excitation_mag_matrix;
	std::vector<int> excitation_mag_dims;
	std::vector<double> excitation_re_matrix;
	std::vector<int> re_dims;
	std::vector<double> excitation_im_matrix;
	std::vector<int> im_dims;
	std::vector<double> excitation_phase_matrix;
	std::vector<int> excitation_phase_dims;
	std::string h5_file_name;
	std::string bodyName;

	std::vector<double> excitation_irf_time;
    Eigen::VectorXd excitation_irf_time_resampled;
    std::vector<double> excitation_irf_matrix;
    std::vector<int> excitation_irf_dims;
    bool is_excitation_irf_time_resampled = false;


	void InitScalar(H5::H5File& file, std::string data_name, double& var);
	void Init1D(H5::H5File& file, std::string data_name, std::vector<double>& var);
	void Init2D(H5::H5File& file, std::string data_name, chrono::ChMatrixDynamic<double>& var);
	void Init3D(H5::H5File& file, std::string data_name, std::vector<double>& var, std::vector<int>& dims);
	void readH5Data();
};