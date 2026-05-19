#include "eg2/libs/typedefs.h"
#include "eg2/libs/miniz.hh"
#include "eg2/sys/report.hh"
#include "inkay/fs/files.hh"
#include <vector>

namespace Inkay {
    namespace ZIP {
        bool Extract(const std::vector<u8>& zipData) {
            mz_zip_archive zip;
            memset(&zip, 0, sizeof(zip));

            if (!mz_zip_reader_init_mem(&zip, zipData.data(), zipData.size(), 0)) {
                SYS::Report::Log("Inkay::ZIP::Extract(): ZIP init failed\n");
                return false;
            }

            int count = (int)mz_zip_reader_get_num_files(&zip);

            for (int i = 0; i < count; i++) {
                mz_zip_archive_file_stat st;

                if (!mz_zip_reader_file_stat(&zip, i, &st)) continue;

                std::string name = st.m_filename;

                usize size = 0;
                void* data = mz_zip_reader_extract_to_heap(&zip, i, &size, 0);

                if (!data || !size) continue;

                if (name.find(".wms") != std::string::npos) {
                    Inkay::Files::WriteWMS(data, size);
                    SYS::Report::Log("Inkay::ZIP::Extract(): WMS extracted\n");
                }

                if (name.find(".wps") != std::string::npos) {
                    Inkay::Files::WriteWPS(data, size);
                    SYS::Report::Log("Inkay::ZIP::Extract(): WPS extracted\n");
                }

                free(data);
            }

            mz_zip_reader_end(&zip);
            return true;
        }
    }
}