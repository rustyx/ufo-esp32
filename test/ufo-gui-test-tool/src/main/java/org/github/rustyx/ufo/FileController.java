package org.github.rustyx.ufo;

import java.io.File;
import java.io.IOException;

import javax.annotation.PostConstruct;

import lombok.extern.slf4j.Slf4j;

import org.apache.commons.io.FileUtils;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;

@Controller
@Slf4j
public class FileController {

    private File rootDir, indexHtml;

    @PostConstruct
    public void init() {
        rootDir = new File(".").getAbsoluteFile();
        for (int i = 0; i < 10 && rootDir != null; ++i) {
            indexHtml = new File(rootDir, "data/index.html");
            if (indexHtml.canRead()) {
                log.info("Found: {}", indexHtml.getAbsolutePath());
                return;
            }
            rootDir = rootDir.getParentFile();
        }
        throw new IllegalStateException("Unable to find data/index.html");
    }

    @GetMapping({ "/", "/index.html" })
    public ResponseEntity<byte[]> index() throws IOException {
        return ResponseEntity.ok(FileUtils.readFileToByteArray(indexHtml));
    }

    @GetMapping("/fonts/{font}")
    public ResponseEntity<byte[]> font(@PathVariable("font") String font) throws IOException {
        return ResponseEntity.ok(FileUtils.readFileToByteArray(new File(rootDir, "data/" + font)));
    }

}
