#pragma once
#include <vector>
#include "PackageSpec.h"
#include "StatusParagraphs.h"
#include "VcpkgPaths.h"
#include "vcpkg_optional.h"
#include "Paragraphs.h"

namespace vcpkg::Dependencies
{
    struct AnyParagraph
    {
        std::vector<PackageSpec> edges() const;

        PackageSpec spec;
        Optional<StatusParagraph> status_paragraph;
        Optional<BinaryParagraph> binary_paragraph;
        Optional<SourceParagraph> source_paragraph;
    };

    enum class RequestType
    {
        UNKNOWN,
        USER_REQUESTED,
        AUTO_SELECTED
    };

    std::string to_output_string(RequestType request_type, const CStringView s);

    enum class InstallPlanType
    {
        UNKNOWN,
        BUILD_AND_INSTALL,
        INSTALL,
        ALREADY_INSTALLED
    };

    struct InstallPlanAction
    {
        InstallPlanAction();
        explicit InstallPlanAction(const AnyParagraph& any_paragraph, const RequestType& request_type);
        InstallPlanAction(const InstallPlanType& plan_type, const RequestType& request_type, Optional<BinaryParagraph> binary_pgh, Optional<SourceParagraph> source_pgh);
        InstallPlanAction(const InstallPlanAction&) = delete;
        InstallPlanAction(InstallPlanAction&&) = default;
        InstallPlanAction& operator=(const InstallPlanAction&) = delete;
        InstallPlanAction& operator=(InstallPlanAction&&) = default;

        InstallPlanType plan_type;
        RequestType request_type;
        Optional<BinaryParagraph> binary_pgh;
        Optional<SourceParagraph> source_pgh;
    };

    struct PackageSpecWithInstallPlan
    {
        static bool compare_by_name(const PackageSpecWithInstallPlan* left, const PackageSpecWithInstallPlan* right);

        PackageSpecWithInstallPlan(const PackageSpec& spec, InstallPlanAction&& plan);

        PackageSpec spec;
        InstallPlanAction plan;
    };

    enum class RemovePlanType
    {
        UNKNOWN,
        NOT_INSTALLED,
        REMOVE
    };

    struct RemovePlanAction
    {
        RemovePlanAction();
        RemovePlanAction(const RemovePlanType& plan_type, const RequestType& request_type);
        RemovePlanAction(const RemovePlanAction&) = delete;
        RemovePlanAction(RemovePlanAction&&) = default;
        RemovePlanAction& operator=(const RemovePlanAction&) = delete;
        RemovePlanAction& operator=(RemovePlanAction&&) = default;

        RemovePlanType plan_type;
        RequestType request_type;
    };

    struct PackageSpecWithRemovePlan
    {
        static bool compare_by_name(const PackageSpecWithRemovePlan* left, const PackageSpecWithRemovePlan* right);

        PackageSpecWithRemovePlan(const PackageSpec& spec, RemovePlanAction&& plan);

        PackageSpec spec;
        RemovePlanAction plan;
    };

    std::vector<PackageSpecWithInstallPlan> create_install_plan(const VcpkgPaths& paths, const std::vector<PackageSpec>& specs, const StatusParagraphs& status_db);

    std::vector<PackageSpecWithRemovePlan> create_remove_plan(const std::vector<PackageSpec>& specs, const StatusParagraphs& status_db);
}
