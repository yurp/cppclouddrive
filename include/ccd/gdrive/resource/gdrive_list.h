
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/executor.h>
#include <ccd/gdrive/details/gdrive_parameters.h>
#include <ccd/gdrive/model/gdrive_file.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

class files;

class list : public executor,
             public details::with_file_parameters<list>
{
    friend class files;

public:
    /// @brief Comma-separated list of bodies of items (files/documents) to which the query applies
    list& set_corpora(std::optional<std::string> x);

    /// @brief The source of files to list ("domain" or "user")
    list& set_corpus(std::optional<std::string> x);

    /// @brief Whether Team Drive items should be included in results
    list& set_include_team_drive_items(std::optional<bool> x);

    /// @brief A comma-separated list of sort keys
    list& set_order_by(std::optional<std::string> x);

    /// @brief The maximum number of files to return per page
    list& set_page_size(std::optional<int> x);

    /// @brief The token for continuing a previous list request on the next page
    list& set_page_token(std::optional<std::string> x);

    /// @brief A query for filtering the file results
    list& set_q(std::optional<std::string> x);

    /// @brief A comma-separated list of spaces to query within the corpus
    list& set_spaces(std::optional<std::string> x);

    /// @brief Whether the requesting application supports Team Drives
    list& set_team_drive_id(std::optional<std::string> x);

    pplx::task<model::file_list> exec();

private:
    list(pplx::task<http_client_ptr> client);

    web::http::http_request build_request() override;

    std::optional<std::string> m_corpora;
    std::optional<std::string> m_corpus;
    std::optional<bool> m_include_team_drive_items;
    std::optional<std::string> m_order_by;
    std::optional<int> m_page_size;
    std::optional<std::string> m_page_token;
    std::optional<std::string> m_q;
    std::optional<std::string> m_spaces;
    std::optional<std::string> m_team_drive_id;
};

}
}
}
