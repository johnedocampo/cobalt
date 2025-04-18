# Copyright (C) 2009 Google Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#    * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#    * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import json
import logging
import unittest

from blinkpy.common.host_mock import MockHost
from blinkpy.common.net.results_fetcher import TestResultsFetcher, Build, filter_latest_builds
from blinkpy.common.net.web_mock import MockWeb
from blinkpy.common.net.web_test_results import Artifact
from blinkpy.common.system.log_testing import LoggingTestCase


class BuilderTest(LoggingTestCase):
    def setUp(self):
        self.set_logging_level(logging.DEBUG)
        self.fetcher = TestResultsFetcher.from_host(MockHost())

    def test_results_url_no_build_number(self):
        self.assertEqual(
            self.fetcher.results_url('Test Builder'),
            'https://test-results.appspot.com/data/layout_results/Test_Builder/results/layout-test-results'
        )

    @unittest.skip('crbug/1234319 disable to unblock the CI')
    def test_results_url_with_build_number(self):
        self.assertEqual(
            self.fetcher.results_url('Test Builder', 10),
            'https://test-results.appspot.com/data/layout_results/Test_Builder/10/layout-test-results'
        )

    def test_results_url_with_build_number_step_name(self):
        self.assertEqual(
            self.fetcher.results_url('Test Builder', 10,
                                     'blink_web_tests (with patch)'),
            'https://test-results.appspot.com/data/layout_results/Test_Builder'
            '/10/blink_web_tests%20%28with%20patch%29/layout-test-results')

    def test_results_url_with_non_numeric_build_number(self):
        with self.assertRaisesRegexp(AssertionError,
                                     'expected numeric build number'):
            self.fetcher.results_url('Test Builder', 'ba5eba11')

    def test_builder_results_url_base(self):
        self.assertEqual(
            self.fetcher.builder_results_url_base('WebKit Mac10.8 (dbg)'),
            'https://test-results.appspot.com/data/layout_results/WebKit_Mac10_8__dbg_'
        )

    def test_accumulated_results_url(self):
        self.assertEqual(
            self.fetcher.accumulated_results_url_base('WebKit Mac10.8 (dbg)'),
            'https://test-results.appspot.com/data/layout_results/WebKit_Mac10_8__dbg_/results/layout-test-results'
        )

    def test_fetch_web_test_results_with_no_results_fetched(self):
        results = self.fetcher.fetch_web_test_results(
            self.fetcher.results_url('B'))
        self.assertIsNone(results)
        self.assertLog([
            'DEBUG: Got 404 response from:\n'
            'https://test-results.appspot.com/data/layout_results/B/results/layout-test-results/failing_results.json\n'
        ])

    def test_fetch_results_with_weird_step_name(self):
        self.fetcher.web = MockWeb(
            urls={
                'https://test-results.appspot.com/testfile?buildnumber=123&'
                'callback=ADD_RESULTS&builder=builder&name=full_results.json':
                b'ADD_RESULTS(' + json.dumps(
                    [{
                        "TestType": "blink_web_tests on Intel GPU (with patch)"
                    }, {
                        "TestType": "base_unittests (with patch)"
                    }]).encode('utf8', 'replace') + b');',
                'https://test-results.appspot.com/data/layout_results/builder/123/'
                'blink_web_tests%20on%20Intel%20GPU%20%28with%20patch%29/'
                'layout-test-results/failing_results.json':
                json.dumps({
                    'tests': {
                        'test.html': {
                            'actual': 'PASS'
                        }
                    },
                }).encode('utf8', 'replace')
            })
        step_name = 'blink_web_tests on Intel GPU (with patch)'
        results = self.fetcher.fetch_results(Build('builder', 123), False,
                                             step_name)
        self.assertIsNot(results.result_for_test('test.html'), None)
        self.assertLog([])

    def test_fetch_results_without_build_number(self):
        self.assertIsNone(self.fetcher.fetch_results(Build('builder', None)))

    def test_fetch_webdriver_results_without_build_number(self):
        self.assertIsNone(
            self.fetcher.fetch_webdriver_test_results(Build('builder', None),
                                                      'bar'))
        self.assertLog(
            ['DEBUG: Builder name or build number or master is None\n'])

    def test_fetch_webdriver_results_without_master(self):
        self.assertIsNone(
            self.fetcher.fetch_webdriver_test_results(Build('builder', 1), ''))
        self.assertLog(
            ['DEBUG: Builder name or build number or master is None\n'])

    def test_fetch_webdriver_test_results_with_no_results(self):
        results = self.fetcher.fetch_webdriver_test_results(
            Build('bar-rel', 123), 'foo.chrome')
        self.assertIsNone(results)
        self.assertLog([
            'DEBUG: Got 404 response from:\n'
            'https://test-results.appspot.com/testfile?buildnumber=123&'
            'master=foo.chrome&builder=bar-rel&'
            'testtype=webdriver_tests_suite+%28with+patch%29&name=full_results.json\n'
        ])

    def test_fetch_webdriver_results_success(self):
        self.fetcher.web = MockWeb(
            urls={
                'https://test-results.appspot.com/testfile?buildnumber=123&'
                'master=foo.chrome&builder=bar-rel&'
                'testtype=webdriver_tests_suite+%28with+patch%29&'
                'name=full_results.json':
                json.dumps({
                    'tests': {
                        'test.html': {
                            'actual': 'PASS'
                        }
                    },
                }).encode('utf8', 'replace'),
            })
        results = self.fetcher.fetch_webdriver_test_results(
            Build('bar-rel', 123), 'foo.chrome')
        self.assertIsNot(results.result_for_test('test.html'), None)
        self.assertLog([])

    def test_get_full_builder_url(self):
        self.assertEqual(
            self.fetcher.get_full_builder_url('https://storage.googleapis.com',
                                              'foo bar'),
            'https://storage.googleapis.com/foo_bar')
        self.assertEqual(
            self.fetcher.get_full_builder_url('https://storage.googleapis.com',
                                              'foo.bar'),
            'https://storage.googleapis.com/foo_bar')
        self.assertEqual(
            self.fetcher.get_full_builder_url('https://storage.googleapis.com',
                                              'foo(bar)'),
            'https://storage.googleapis.com/foo_bar_')

    def test_gather_results(self):
        self.fetcher.web.append_prpc_response({
            'testResults': [{
                'name':
                ('invocations/task-chromium-swarm.appspot.com-6139bb/'
                 'tests/ninja:%2F%2F:blink_web_tests%2Fshould-pass.html/'
                 'results/033e-aaaa'),
                'testId':
                'ninja://:blink_web_tests/should-pass.html',
                'status':
                'FAIL',
                'tags': [{
                    'key': 'web_tests_actual_image_hash',
                    'value': '3f765a7',
                }],
            }, {
                'name':
                ('invocations/task-chromium-swarm.appspot.com-6139bb/'
                 'tests/ninja:%2F%2F:blink_web_tests%2Fshould-pass.html/'
                 'results/033e-bbbb'),
                'testId':
                'ninja://:blink_web_tests/should-pass.html',
                'status':
                'PASS',
                'expected':
                True,
            }, {
                'name': ('invocations/task-chromium-swarm.appspot.com-6139bb/'
                         'tests/ninja:%2F%2F:blink_wpt_tests%2F'
                         'external%2Fwpt%2Ftimeout.html/'
                         'results/033e-cccc'),
                'testId':
                'ninja://:blink_wpt_tests/external/wpt/timeout.html',
                'status':
                'ABORT',
                'expected':
                True,
            }],
        })
        self.fetcher.web.append_prpc_response({
            'artifacts': [{
                'name':
                ('invocations/task-chromium-swarm.appspot.com-6139bb/'
                 'tests/ninja:%2F%2F:blink_web_tests%2Fshould-pass.html/'
                 'results/033e-aaaa/'
                 'artifacts/actual_image'),
                'artifactId':
                'actual_image',
                'fetchUrl':
                'https://results.usercontent.cr.dev/actual_image',
            }],
        })
        results = self.fetcher.gather_results(Build('linux-rel', 9000, '1234'),
                                              'blink_web_tests (with patch)',
                                              True, False)

        result = results.result_for_test('should-pass.html')
        self.assertEqual(result.actual_results(), ['FAIL', 'PASS'])
        self.assertFalse(result.did_run_as_expected())
        self.assertEqual(
            result.baselines_by_suffix(), {
                'png': [
                    Artifact('https://results.usercontent.cr.dev/actual_image',
                             '3f765a7'),
                ],
            })

        result = results.result_for_test('external/wpt/timeout.html')
        self.assertEqual(result.actual_results(), ['TIMEOUT'])
        self.assertTrue(result.did_run_as_expected())

    def test_fetch_wpt_report_urls(self):
        self.fetcher.web.append_prpc_response({
            'artifacts': [{
                'name': 'report1',
                'artifactId': 'wpt_reports_dada.json',
                'fetchUrl': 'https://a.b.c/report1.json',
                'sizeBytes': '8472164',
            }, {
                'name': 'report2',
                'artifactId': 'wpt_reports_dada.json',
                'fetchUrl': 'https://a.b.c/report2.json',
                'sizeBytes': '8455564',
            }, {
                'name': 'other',
                'artifactId': 'other_dada.json',
                'fetchUrl': 'https://a.b.c/other.json',
                'sizeBytes': '9845',
            }],
        })
        self.assertEqual(
            self.fetcher.fetch_wpt_report_urls('31415926535'),
            ['https://a.b.c/report1.json', 'https://a.b.c/report2.json'])

        self.fetcher.web.append_prpc_response({
            'artifacts': [{
                'name': 'other',
                'artifactId': 'other_dada.json',
                'fetchUrl': 'https://a.b.c/other.json',
                'sizeBytes': '9845',
            }],
        })
        self.assertEqual(self.fetcher.fetch_wpt_report_urls('31415926535'), [])

        self.fetcher.web.append_prpc_response({})
        self.assertEqual(self.fetcher.fetch_wpt_report_urls('31415926535'), [])


class TestResultsFetcherHelperFunctionTest(unittest.TestCase):
    def test_filter_latest_jobs_empty(self):
        self.assertEqual(filter_latest_builds([]), [])

    def test_filter_latest_jobs_higher_build_first(self):
        self.assertEqual(
            filter_latest_builds(
                [Build('foo', 5),
                 Build('foo', 3),
                 Build('bar', 5)]),
            [Build('bar', 5), Build('foo', 5)])

    def test_filter_latest_jobs_higher_build_last(self):
        self.assertEqual(
            filter_latest_builds(
                [Build('foo', 3),
                 Build('bar', 5),
                 Build('foo', 5)]),
            [Build('bar', 5), Build('foo', 5)])

    def test_filter_latest_jobs_no_build_number(self):
        self.assertEqual(
            filter_latest_builds([Build('foo', 3),
                                  Build('bar'),
                                  Build('bar')]),
            [Build('bar'), Build('foo', 3)])
