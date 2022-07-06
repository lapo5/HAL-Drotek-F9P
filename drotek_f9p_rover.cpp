#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <iostream>
#include <string>
#include <exception>

#include <math.h>
#include "HAL_Sirius_F9P_Rover.cc"

namespace py = pybind11;

class PyHALDrotekF9P{
	private:
		SiriusF9P_Rover* hal_gps;
		bool connected;
	public:
	PyHALDrotekF9P(){}

	~PyHALDrotekF9P(){
		if (connected) hal_gps->disconnect();
	}

	bool init(std::string port){
		try{
			hal_gps = new SiriusF9P_Rover(port, 9600, READ_GNS | READ_GST);
			connected = hal_gps->isPortOpen();
			return connected;
		}
		catch(exception& e){
			std::cout << "Exception: " << e.what() << std::endl;
			return false;
		}
	}

	std::string get_gnss_name(){
		return "Drotek-F9P";
	}

	bool shutdown(){
		hal_gps->disconnect();
		return true;
	}

	void get_gnss_info(py::array_t<double, py::array::c_style | py::array::forcecast> gnss_status){
		hal_gps->readPacket();
		auto gnss_status_ptr = (double*)gnss_status.request().ptr;
		double* res_gps = hal_gps->getStatus();

		gnss_status_ptr[0] = res_gps[0];
		gnss_status_ptr[1] = res_gps[1];
		gnss_status_ptr[2] = res_gps[2];
		gnss_status_ptr[3] = res_gps[3];
		gnss_status_ptr[4] = res_gps[4];
		gnss_status_ptr[5] = res_gps[5];

	}

	bool has_fix(){
		hal_gps->readPacket();
		hal_gps->getStatus();
		return hal_gps->hasFix();
	}

	constexpr bool has_rtcm() const {
		return true;
	}

	void put_rtcm_msg(py::array_t<unsigned char, py::array::c_style | py::array::forcecast> rtcm_msg, int rtcm_size){
		auto rtcm_buf = (const char*)rtcm_msg.request().ptr;
		auto gil_release = py::gil_scoped_release();
		hal_gps->writePacket(rtcm_buf, rtcm_size);
	}
};

PYBIND11_MODULE(drotek_f9p_rover, m) {
    py::class_<PyHALDrotekF9P>(m, "HAL")
		.def(py::init<>())
		.def("init", &PyHALDrotekF9P::init)
		.def("get_gnss_name", &PyHALDrotekF9P::get_gnss_name)
		.def("shutdown", &PyHALDrotekF9P::shutdown)
		.def("get_gnss_info", &PyHALDrotekF9P::get_gnss_info)
		.def("has_fix", &PyHALDrotekF9P::has_fix)
		.def("has_rtcm", &PyHALDrotekF9P::has_rtcm)
		.def("put_rtcm_msg", &PyHALDrotekF9P::put_rtcm_msg);
}
