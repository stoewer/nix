// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <limits>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include <pandora.hpp>

using namespace pandora;

class TestNamedEntityWithSources : public CPPUNIT_NS::TestFixture {

private:

    CPPUNIT_TEST_SUITE(TestNamedEntityWithSources);
    CPPUNIT_TEST(testAddRemoveSource);
    CPPUNIT_TEST_SUITE_END ();

    File *f1;
    std::string s_id1, s_id2, b_id;
public:

    void setUp();
    void tearDown();
    void testAddRemoveSource();

};
