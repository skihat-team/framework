<?php
/**
 * paginate.inc 提供视图的分页助手方法声明。
 *
 * @copyright   2012-2013 Skihat team (skihat.cn)
 * @license     http://skihat.cn/license.html
 * @author      Skihat Team
 *
 * @package     skihat.views._helpers
 * @version     1.0
 *
 * @create      2012-10-16
 * @modify      2013-09-13
 */

/**
 * 根据$page和$ruleName生成分页URL地址。
 *
 * @param int         $page     分页码。
 * @param bool|string $ruleName 规则名称。
 *
 * @return string
 */
function paginate_url($page, $ruleName = false) {
    return Router::url(array(
        SKIHAT_PARAM_PAGINATE => $page,
        SKIHAT_PARAM_PAGINATE_DIR => 'asc',
        SKIHAT_PARAM_PAGINATE_SORT => null,
        Router::FULL_URL => true,
        Router::RULE_NAME => $ruleName));
}

/**
 * paginate_url的直接打印版本。
 *
 * @see paginate_url
 */
function _paginate_url($page, $ruleName = false) {
    echo paginate_url($page, $ruleName);
}

/**
 * 根据$title、$page和$ruleName生成分页连接。
 *
 * @param string $title    显示标题。
 * @param int    $page     分页数量。
 * @param bool   $ruleName 路由规则。
 *
 * @return string
 */
function paginate_link($title, $page, $ruleName = false) {
    return '<a href="' . paginate_url($page, $ruleName) . '">' . $title . '</a>';
}

/**
 * paginate_link的HTML直接输出版本。
 *
 * @see paginate_link
 */
function _paginate_link($title, $page, $ruleName = false) {
    echo paginate_link($title, $page, $ruleName);
}

/**
 * 根据$fetch和$options 显示数字分页HTML。
 *
 * @param ModelFetch $fetch
 * @param array      $options
 *
 * @return string
 */
function paginate_numbers(ModelFetch $fetch, array $options = array()) {
    # 如果模型查找组件不包含分页信息，则直接返回空字符串。
    if (!($paginate = $fetch->extra('paginate'))) return '';
    if ($paginate['page-all'] <= 1) return '';

    $class = array_key_value($options, 'class', 'pagination');
    $numbers = array_key_value($options, 'numbers', 9);
    $ruleName = array_key_value($options, 'rule', false);

    $html = sprintf('<ul class="%s">', $class);

    # 生成第一页和上一页。
    if ($paginate['size'] == 1 || $paginate['page-all'] <= 1) {
        $html .= '<li class="first"><span>&lt;&lt;</span></li>';
        $html .= '<li class="prov"><span>&lt;</span></li>';
    } else {
        $html .= sprintf('<li class="first">%s</li>', paginate_link('&lt;&lt', 1, $ruleName));
        $html .= sprintf('<li class="prov">%s</li>', paginate_link('&lt;', $paginate['page'] - 1, $ruleName));
    }

    # 开始处理分页数字列表。
    $startPage = 1;
    $currPage = $paginate['page'];
    $endPage = $paginate['page-all'];

    # 如果分页总数大于显示数量。
    if ($endPage > $numbers) {
        $halfPage = floor($numbers / 2);

        if ($currPage < $halfPage) {
            $endPage = $numbers;
        } else if ($currPage + $halfPage >= $endPage) {
            $startPage = $endPage - $numbers;
        } else {
            $startPage = $currPage - $halfPage;
            $endPage = $currPage + $halfPage;
        }
    }

    # 生成页码。
    for (; $startPage <= $endPage; ++$startPage) {
        # 如果开始页与当前页相同。
        if ($startPage == $currPage) {
            $html .= sprintf('<li class="active"><span>%s</span></li>', $startPage);
        } else {
            $html .= sprintf('<li>%s</li>', paginate_link($startPage, $startPage, $ruleName));
        }
    }

    # 生成下一页和最后一页。
    if ($paginate['page'] >= $paginate['page-all']) {
        $html .= '<li class="next"><span >&gt;</span></li>';
        $html .= '<li class="last"><span >&gt;&gt;</span></li>';
    } else {
        $html .= sprintf('<li class="next">%s</li>', paginate_link('&gt;', $paginate['page'] + 1, $ruleName));
        $html .= sprintf('<li class="last">%s</li>', paginate_link('&gt;&gt;', $paginate['page-all'], $ruleName));
    }

    return $html . '</ul>';
}

/**
 * paginate_numbers 的直接输出版本。
 *
 * @see paginate_numbers
 */
function _paginate_numbers(ModelFetch $fetch, array $options = array()) {
    echo paginate_numbers($fetch, $options);
}

/**
 * 根据$title、$sort和$dir生成分页排序。
 *
 * @param string      $title    显示标题。
 * @param string      $sort     排序字段。
 * @param bool|string $dir      排序方式，如果排序方式为false则表示自动。
 * @param string|bool $ruleName 路由规则名称。
 *
 * @return string
 */
function paginate_sort_link($title, $sort, $dir = false, $ruleName = false) {
    $request = Application::request();
    $currSort = $request->query(SKIHAT_PARAM_PAGINATE_SORT);
    $currDir = $request->query(SKIHAT_PARAM_PAGINATE_DIR);

    if ($dir == false) {
        if ($currSort == $sort) {
            $dir = ($currDir == 'asc') ? 'desc' : 'asc';
        } else {
            $dir = 'asc';
        }
    }

    $class = ($currSort == $sort) ? 'active ' . $dir : $dir;
    $url = Router::url(array(
        SKIHAT_PARAM_PAGINATE => 1,
        SKIHAT_PARAM_PAGINATE_DIR => $dir,
        SKIHAT_PARAM_PAGINATE_SORT => $sort,
        Router::FULL_URL => true,
        Router::RULE_NAME => $ruleName));

    return '<a href="' . $url . '" class="' . $class . '">' . $title . '</a>';
}

/**
 * paginate_sort_link的直接输出版本。
 *
 * @see paginate_sort_link
 */
function _paginate_sort_link($title, $sort, $dir = false, $ruleName = false) {
    echo paginate_sort_link($title, $sort, $dir, $ruleName);
}

/**
 * 根据$fetch和$options生成分页导航HTML。
 *
 * @param ModelFetch $fetch   模型检索器。
 * @param array      $options 选项。
 *
 * @return string
 */
function paginate_navigator(ModelFetch $fetch, $options = array()) {
    # 如果包含有分页信息。
    if (!($paginate = $fetch->extra('paginate'))) return '';
    if ($paginate['page-all'] <= 1) return '';

    $provTitle = isset($options['prov-title']) ? $options['prov-title'] : '&lt';
    $nextTitle = isset($options['next-title']) ? $options['next-title'] : '&gt';
    $ruleName = isset($options['rule']) ? $options['rule'] : false;
    $class = isset($options['class']) ? $options['class'] : 'nav';

    if ($paginate['page'] <= 1) {
        $html = sprintf('<a href="javascript:void(0);" class="prov %s">%s</a>', $class, $provTitle);
    } else {
        $html =
            sprintf('<a href="%s" class="prov %s">%s</a>', paginate_url($paginate['page'] - 1, $ruleName), $class, $provTitle);
    }

    $html .= sprintf('<span class="%s">%d/%d</span>', $class, $paginate['page'], $paginate['page-all']);

    if ($paginate['page'] >= $paginate['page-all']) {
        $html .= sprintf('<a href="javascript:void(0);" class="next %s">%s</a>', $class, $nextTitle);
    } else {
        $html .= sprintf('<a href="%s" class="next %s">%s</a>', paginate_url($paginate['page'] + 1, $ruleName), $class, $nextTitle);
    }

    return $html;
}

/**
 * @see paginate_navigator
 */
function _paginate_navigator(ModelFetch $fetch, $options = array()) {
    echo paginate_navigator($fetch, $options);
}
