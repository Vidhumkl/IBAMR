// Filename: IBTK_Init.cpp
//
// Copyright (c) 2002-2017, Boyce Griffith
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of The University of North Carolina nor the names of
//      its contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

/////////////////////////////// INCLUDES /////////////////////////////////////

#include "ibtk/IBTK_Init.h"
#include "ibtk/app_namespaces.h"

namespace IBTK
{
IBTK_Init::IBTK_Init(int argc, char** argv, IBTK_MPI::comm communicator, char* petsc_file, char* petsc_help)
{
#ifdef IBTK_HAVE_LIBMESH
    d_libmesh_init = std::make_shared<LibMeshInit>(argc, argv);
    libMesh::ReferenceCounter::disable_print_counter_info();
#else
    // We need to initialize PETSc.
    // TODO: Should we include a way to pass these extra options to PetscInitialize?
    PetscInitialize(&argc, &argv, petsc_file, petsc_help);
#endif
#if SAMRAI_VERSION_MAJOR > 2
    SAMRAIManager::initialize();
    SAMRAI_MPI::init(comm);
#else
    SAMRAI_MPI::setCommunicator(communicator);
#endif
    IBTK_MPI::setCommunicator(communicator);
    SAMRAIManager::startup();
}

IBTK_Init::~IBTK_Init()
{
    pout << "IBTK_Init destructor called. Shutting down libraries.\n";
    SAMRAIManager::shutdown();
#ifndef IBTK_HAVE_LIBMESH
    PetscFinalize();
#endif
}

} // namespace IBTK
