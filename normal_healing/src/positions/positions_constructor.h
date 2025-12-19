#ifndef POSITIONS_H_
#define POSITIONS_H_


#include "biodynamo.h"


namespace bdm {

	std::vector<Real3> krt_pos;
	std::vector<Real3> fbr_pos;
	std::vector<Real3> mac_pos;
	std::vector<Real3> plt_pos;
	
	int spacing = 3;                         
	for (real_t x = 0; x <= 100; x += spacing) {
		for (real_t y = 0; y <= 100; y += spacing) {
			for (real_t z = 0; z <= 100; z += spacing) {
				if (y > 70 && (y < abs(2*x - 100) + 40)) {
					Real3 pos = {x, y, z};
					auto* r = Simulation::GetActive()->GetRandom();
					Real3 random_vector = r->UniformArray<3>(-1, 1);
					random_vector.Normalize();
					pos = random_vector + pos;
					auto* krt = krt_builder(pos);
					krt_pos.push_back(pos);
					rm->AddAgent(krt);
					pos = random_vector - pos;
					//continue;
					
				}
				else if (y > abs(2*x - 100) + 30 && y < abs(2*x - 100) + 40) {
					
					Real3 pos = {x, y, z};
					auto* r = Simulation::GetActive()->GetRandom();    //fibroblasts
					Real3 random_vector = r->UniformArray<3>(-1, 1);
					random_vector.Normalize();
					pos = random_vector + pos;
					fbr_pos.push_back(pos);
					rm->AddAgent(fbr_builder(pos));
					
					r = Simulation::GetActive()->GetRandom();   //macrophages
					random_vector = r->UniformArray<3>(-1, 1);
					random_vector.Normalize();
					pos = random_vector + pos;
					mac_pos.push_back(pos);
					rm->AddAgent(mac_builder(pos));	
				}
				else if (y > abs(2*x - 100) + 40) {
					Real3 pos = {x, y, z};
					auto* r = Simulation::GetActive()->GetRandom(); //platelets
					Real3 random_vector = r->UniformArray<3>(-1, 1);
					random_vector.Normalize();
					pos = random_vector + pos;
					plt_pos.push_back(pos);
					rm->AddAgent(plt_builder(pos));	
				}
			}
		}
	};
        
      std::ofstream file_krt("krt_pos.txt");
	  std::ofstream file_plt("plt_pos.txt");
	  std::ofstream file_fbr("fbr_pos.txt");
	  std::ofstream file_mac("mac_pos.txt");
	  
	  if (!file_krt.is_open()) {
	    std::cerr << "Error: could not open file for writing\n";
	    return 1;
	  }

	  for (const auto& pos : krt_pos) {
	    file_krt << std::fixed << std::setprecision(6);
	    file_krt << pos[0] << " " << pos[1] << " " << pos[2] << "\n";
	 }
	  for (const auto& pos : plt_pos) {
	    file_plt << std::fixed << std::setprecision(6);
	    file_plt << pos[0] << " " << pos[1] << " " << pos[2] << "\n";
	 }
	  for (const auto& pos : mac_pos) {
	    file_fbr << std::fixed << std::setprecision(6);
	    file_fbr << pos[0] << " " << pos[1] << " " << pos[2] << "\n";
	  }
	  for (const auto& pos : fbr_pos) {
	    file_mac << std::fixed << std::setprecision(6);
	    file_mac << pos[0] << " " << pos[1] << " " << pos[2] << "\n";
	  }

	  file_krt.close(); 
	  file_plt.close(); 
	  file_mac.close(); 
	  file_fbr.close();    

} //nmpsce bdm
#endif
