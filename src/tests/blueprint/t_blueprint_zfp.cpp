//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2019, Lawrence Livermore National Security, LLC.
//
// Produced at the Lawrence Livermore National Laboratory
//
// LLNL-CODE-666778
//
// All rights reserved.
//
// This file is part of Conduit.
//
// For details, see https://lc.llnl.gov/conduit/.
//
// Please also read conduit/LICENSE
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the disclaimer below.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the disclaimer (as noted below) in the
//   documentation and/or other materials provided with the distribution.
//
// * Neither the name of the LLNS/LLNL nor the names of its contributors may
//   be used to endorse or promote products derived from this software without
//   specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL LAWRENCE LIVERMORE NATIONAL SECURITY,
// LLC, THE U.S. DEPARTMENT OF ENERGY OR CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
// IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//-----------------------------------------------------------------------------
///
/// file: t_blueprint_zfp.cpp
///
//-----------------------------------------------------------------------------

#include "conduit.hpp"
#include "conduit_blueprint.hpp"

#include <iostream>
#include "gtest/gtest.h"

using namespace conduit;

// some CI do not have zfp available
// these tests look at dtypes and Npde entries, so contents don't matter

void set_zfparray_node_entries(Node& result, uint8* header, size_t n_header, uint8* compressed_data, size_t n_compressed_data) {
  result[blueprint::zfparray::ZFP_HEADER_FIELD_NAME].set(header, n_header);
  result[blueprint::zfparray::ZFP_COMPRESSED_DATA_FIELD_NAME].set(compressed_data, n_compressed_data);
}

//-----------------------------------------------------------------------------
TEST(conduit_blueprint_zfp, zfp_verify_valid_zfparray)
{
    size_t n_header = 4;
    uint8 * header = new uint8[n_header]();

    size_t n_compressed_data = 4;
    uint8 * compressed_data = new uint8[n_compressed_data]();

    Node result, info;
    set_zfparray_node_entries(result, header, n_header, compressed_data, n_compressed_data);

    EXPECT_TRUE(blueprint::zfparray::verify(result, info));

    delete [] header;
    delete [] compressed_data;
}

//-----------------------------------------------------------------------------
TEST(conduit_blueprint_zfp, zfp_verify_invalid_zfparray_without_header)
{
    size_t n_header = 4;
    uint8 * header = new uint8[n_header]();

    size_t n_compressed_data = 4;
    uint8 * compressed_data = new uint8[n_compressed_data]();

    Node result, info;
    set_zfparray_node_entries(result, header, n_header, compressed_data, n_compressed_data);

    EXPECT_TRUE(blueprint::zfparray::verify(result, info));

    // remove header node
    EXPECT_TRUE(result.has_child(blueprint::zfparray::ZFP_HEADER_FIELD_NAME));
    result.remove(blueprint::zfparray::ZFP_HEADER_FIELD_NAME);

    EXPECT_FALSE(blueprint::zfparray::verify(result, info));

    delete [] header;
    delete [] compressed_data;
}

//-----------------------------------------------------------------------------
TEST(conduit_blueprint_zfp, zfp_verify_invalid_zfparray_without_compressed_data)
{
    size_t n_header = 4;
    uint8 * header = new uint8[n_header]();

    size_t n_compressed_data = 4;
    uint8 * compressed_data = new uint8[n_compressed_data]();

    Node result, info;
    set_zfparray_node_entries(result, header, n_header, compressed_data, n_compressed_data);

    EXPECT_TRUE(blueprint::zfparray::verify(result, info));

    // remove compressed-data node
    EXPECT_TRUE(result.has_child(blueprint::zfparray::ZFP_COMPRESSED_DATA_FIELD_NAME));
    result.remove(blueprint::zfparray::ZFP_COMPRESSED_DATA_FIELD_NAME);

    EXPECT_FALSE(blueprint::zfparray::verify(result, info));

    delete [] header;
    delete [] compressed_data;
}

//-----------------------------------------------------------------------------
TEST(conduit_blueprint_zfp, zfp_verify_invalid_zfparray_with_incorrect_header_dtype)
{
    size_t n_header = 4;
    uint8 * header = new uint8[n_header]();

    size_t n_compressed_data = 4;
    uint8 * compressed_data = new uint8[n_compressed_data]();

    Node result, info;
    set_zfparray_node_entries(result, header, n_header, compressed_data, n_compressed_data);

    EXPECT_TRUE(blueprint::zfparray::verify(result, info));

    // remove header node
    EXPECT_TRUE(result.has_child(blueprint::zfparray::ZFP_HEADER_FIELD_NAME));
    result.remove(blueprint::zfparray::ZFP_HEADER_FIELD_NAME);

    // re-add header node as a double
    double dummy = 4.4;
    result[blueprint::zfparray::ZFP_HEADER_FIELD_NAME] = dummy;

    EXPECT_FALSE(blueprint::zfparray::verify(result, info));

    delete [] header;
    delete [] compressed_data;
}

//-----------------------------------------------------------------------------
TEST(conduit_blueprint_zfp, zfp_verify_invalid_zfparray_with_incorrect_compressed_data_dtype)
{
    size_t n_header = 4;
    uint8 * header = new uint8[n_header]();

    size_t n_compressed_data = 4;
    uint8 * compressed_data = new uint8[n_compressed_data]();

    Node result, info;
    set_zfparray_node_entries(result, header, n_header, compressed_data, n_compressed_data);

    EXPECT_TRUE(blueprint::zfparray::verify(result, info));

    // remove compressed-data node
    EXPECT_TRUE(result.has_child(blueprint::zfparray::ZFP_COMPRESSED_DATA_FIELD_NAME));
    result.remove(blueprint::zfparray::ZFP_COMPRESSED_DATA_FIELD_NAME);

    // re-add compressed-data node as a double
    double dummy = 4.4;
    result[blueprint::zfparray::ZFP_COMPRESSED_DATA_FIELD_NAME] = dummy;

    EXPECT_FALSE(blueprint::zfparray::verify(result, info));

    delete [] header;
    delete [] compressed_data;
}

