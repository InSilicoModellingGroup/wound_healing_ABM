#ifndef VORONOI_EDGES3D_H_
#define VORONOI_EDGES3D_H_


#include "biodynamo.h"
#include <vector>
#include <random>
#include <limits>
#include <cmath>
#include <algorithm>


namespace bdm {

static const std::vector<Real3> voronoi_seeds = {
  {58.24,94.85,81.44}, {39.18,84.02,42.78}, {12.37,27.84,21.13}, {91.75,54.12,62.89}, {34.02,21.13,92.78},
  {61.34,37.63,46.91}, {97.42,18.56,17.01}, {67.52,78.35,82.47}, {11.34,42.78,70.10}, {87.63,90.21,43.30},
  {33.51,45.88,79.90}, {62.89,57.73,28.61}, {89.69,90.72,52.06}, {68.04,28.35,78.86}, {30.41,57.22,86.91},
  {39.69,93.81,23.20}, {76.80,14.43,76.80}, {54.12,87.11,31.96}, {23.71,64.95,97.94}, {88.65,46.91,58.25},
  {20.10,29.90,24.74}, {98.45,76.29,28.61}, {13.92,55.15,82.99}, {71.13,13.92,26.55}, {49.48,39.69,92.27},
  {20.10,94.33,47.94}, {92.27,65.46,76.80}, {27.84,22.68,73.19}, {84.02,87.63,46.91}, {62.89,02.06,61.86},
  {26.80,38.14,10.82}, {74.23,49.48,77.84}, {54.12,86.08,87.11}, {00.00,61.34,70.62}, {80.41,22.16,18.04},
  {61.34,34.02,25.52}, {36.08,93.30,10.31}, {97.94,78.35,82.47}, {79.90,31.44,41.75}, {18.04,76.80,05.15},
  {67.52,85.57,97.42}, {88.14,09.28,58.25}, {49.48,57.22,40.21}, {29.90,78.86,87.63}, {94.33,67.01,34.54},
  {81.44,09.28,48.97}, {23.20,56.19,89.17}, {58.76,16.49,14.43}, {12.89,82.47,54.12}, {80.41,90.72,30.41},
  {36.60,64.95,24.74}, {92.27,57.22,48.97}, {54.12,14.43,86.08}, {21.65,94.85,62.89}, {68.04,30.41,36.60},
  {42.27,92.27,72.16}, {87.63,54.12,26.55}, {75.26,87.63,10.31}, {08.25,67.01,56.19}, {30.41,82.47,77.32},
  {68.04,25.52,13.92}, {95.88,89.69,76.29}, {52.06,86.60,30.93}, {38.14,07.22,75.77}, {72.68,97.94,56.19},
  {11.34,54.64,38.66}, {84.54,29.90,72.68}, {26.29,16.49,92.78}, {46.39,73.71,89.17}, {64.95,40.72,20.62},
  {77.84,93.81,31.96}, {18.04,39.18,12.89}, {44.85,88.14,81.44}, {79.38,07.73,95.88}, {63.92,97.42,46.39},
  {05.15,76.29,42.27}, {71.13,34.02,35.57}, {89.69,27.32,74.74}, {34.02,82.47,14.43}, {94.33,13.92,56.70},
  {48.45,42.78,96.39}, {78.35,65.46,28.61}, {14.43,85.57,87.63}, {98.45,33.51,10.31}, {53.61,51.03,24.74},
  {30.93,92.78,92.27}, {62.89,37.63,28.08}, {47.94,21.13,22.16}, {96.39,81.44,90.72}, {23.71,63.92,12.37},
  {83.00,78.35,17.53}, {13.92,24.50,92.27}, {47.94,10.31,50.00}, {16.49,53.09,25.52}, {97.94,75.26,72.68},
  {09.28,88.14,20.62}, {64.43,39.18,54.64}, {90.72,57.22,32.99}, {30.93,47.43,79.90}, {72.16,97.42,79.90},
  {83.00,60.82,12.37}, {13.92,74.23,82.99}, {67.52,30.41,22.16}, {97.42,67.01,73.19}, {35.57,86.08,78.35},
  {53.61,49.48,91.24}, {62.37,18.04,78.86}, {03.61,07.73,07.73}
};



// Spawn agents on 3D Voronoi *edges* (ridges where 3 regions meet).
// AgentT must be constructible as: new AgentT(Real3 pos)
//
// seeds              : list of Voronoi sites in 3D (your fixed seeds)
// min, max           : domain bounds (microns)
// grid_dx            : voxel step; smaller -> more precise edges, more points
// tie_epsilon        : how equal the 3 nearest distances must be (microns)
// gap_epsilon        : how much farther the 4th must be (microns) to avoid faces
// jitter             : random ±jitter to de-voxelize the lines (microns)
// spawn_every_nth    : thin out points along the edges (>=1; 1 = keep all)
// rng_seed           : RNG seed for reproducibility
//
// Returns number of agents spawned.
// Stricter: spawn only where p is close to the *intersection* of two bisector planes
// -> much more line-like output (Voronoi 3D edges)
template <typename epithelial_cell>
inline std::vector<Real3> GetVoronoiEdgePositions(
    const std::vector<Real3>& seeds,
    const Real3& min, const Real3& max,
    real_t grid_dx, real_t plane_eps, real_t gap_epsilon,
    real_t jitter, int every_nth, uint64_t rng_seed)
{
  std::vector<Real3> positions;  // store all edge points
  positions.reserve(100000);     // optional: preallocate

  std::mt19937_64 rng(rng_seed);
  std::uniform_real_distribution<real_t> jdist(-jitter, jitter);

  size_t counter = 0;

  for (real_t z = min[2]; z <= max[2]; z += grid_dx) {
    for (real_t y = min[1]; y <= max[1]; y += grid_dx) {
      for (real_t x = min[0]; x <= max[0]; x += grid_dx) {
        const Real3 p = {x, y, z};

        // find nearest seeds, same as before
        real_t d2_1 = std::numeric_limits<real_t>::infinity();
        real_t d2_2 = std::numeric_limits<real_t>::infinity();
        real_t d2_3 = std::numeric_limits<real_t>::infinity();
        int i1=-1, i2=-1, i3=-1;

        for (int i = 0; i < static_cast<int>(seeds.size()); ++i) {
          const auto& s = seeds[i];
          const real_t dx = p[0] - s[0];
          const real_t dy = p[1] - s[1];
          const real_t dz = p[2] - s[2];
          const real_t d2 = dx*dx + dy*dy + dz*dz;

          if (d2 < d2_1) { d2_3=d2_2; i3=i2; d2_2=d2_1; i2=i1; d2_1=d2; i1=i; }
          else if (d2 < d2_2) { d2_3=d2_2; i3=i2; d2_2=d2; i2=i; }
          else if (d2 < d2_3) { d2_3=d2; i3=i; }
        }

        const real_t d1 = std::sqrt(d2_1);
        const real_t d3 = std::sqrt(d2_3);
        if ((d3 - d1) > gap_epsilon) continue;

        // Check bisector planes
        const Real3& a = seeds[i1];
        const Real3& b = seeds[i2];
        const Real3& c = seeds[i3];
        auto sqr = [](real_t v){ return v*v; };

        Real3 n_ab = { b[0]-a[0], b[1]-a[1], b[2]-a[2] };
        Real3 n_ac = { c[0]-a[0], c[1]-a[1], c[2]-a[2] };
        const real_t len_ab = std::sqrt(sqr(n_ab[0])+sqr(n_ab[1])+sqr(n_ab[2]));
        const real_t len_ac = std::sqrt(sqr(n_ac[0])+sqr(n_ac[1])+sqr(n_ac[2]));
        if (len_ab==0 || len_ac==0) continue;

        const real_t rhs_ab = (sqr(b[0])+sqr(b[1])+sqr(b[2]) - (sqr(a[0])+sqr(a[1])+sqr(a[2]))) * 0.5;
        const real_t rhs_ac = (sqr(c[0])+sqr(c[1])+sqr(c[2]) - (sqr(a[0])+sqr(a[1])+sqr(a[2]))) * 0.5;

        const real_t dist_ab = std::abs((n_ab[0]*p[0]+n_ab[1]*p[1]+n_ab[2]*p[2])-rhs_ab)/len_ab;
        const real_t dist_ac = std::abs((n_ac[0]*p[0]+n_ac[1]*p[1]+n_ac[2]*p[2])-rhs_ac)/len_ac;

        if (dist_ab <= plane_eps && dist_ac <= plane_eps) {
          if ((counter++ % every_nth) == 0) {
            Real3 q = {x + jdist(rng), y + jdist(rng), z + jdist(rng)};
            positions.push_back(q);
          }
        }
      }
    }
  }

  return positions;  // all Voronoi-edge coordinates
}

}  // namespace bdm

#endif  // VORONOI_EDGES3D_H_

